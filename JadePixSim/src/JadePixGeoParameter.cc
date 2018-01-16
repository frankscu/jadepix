//---------------------------------------------------------------------------//
//      BOOST --- BESIII Object_Oriented Simulation Tool                     //
//---------------------------------------------------------------------------//
//Description: Handle database I/O and user interface 
//             for MDC geometry parameters
//Author: Yuan Ye(yuany@mail.ihep.ac.cn)
//Created: 4 Dec, 2003
//Modified:
//Comment: Used in "JadePix" now, should be insert in framwork later
//         The units are "mm" and "rad". 
//         Datum plane is the East Endplane of MDC.
//---------------------------------------------------------------------------//

#include <iostream>
#include <fstream>

#include <CLHEP/Units/PhysicalConstants.h>
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4UnitsTable.hh"

#include "JadePixGeoParameter.hh"
#include "globals.hh"
#include <math.h>
#include <cstdlib>

JadePixGeoParameter * JadePixGeoParameter::fSptGeo=0;

JadePixGeoParameter * JadePixGeoParameter::GetGeo(void){
    if (! fSptGeo) fSptGeo = new JadePixGeoParameter();
    return fSptGeo;
}

void JadePixGeoParameter::Initialize(void){
    layerNo=1;
}

void JadePixGeoParameter::InitFromFile(){

    std::string geometryFilePath = "/home/jingmq/jadepix/JadePixSim";
    geometryFilePath += "/model/Spt.txt";
    std::ifstream fin(geometryFilePath.c_str(),ios::in);

    if(!fin.good()){
        cout<<"Error::spt parameters file not exist"<<endl;
        return;
    }

    int rowNo, colNo, nAdcBit, adcR, _digiMethod;
    double chipL,chipW,asicTh,sensorTh,substrTh,pitchR,pitchC,diodeOffsetX,diodeOffsetY,diodeSize,_cce,_enc;
    int ladderNo,chipNo;
    double ssL,ssW,epoxyT,kptT,mfT,cfT,_R,_Phi,_PhaseAngle;
    double segInnR,segOutR,segL,segZ;

    string name,line;

    std::getline(fin,line);
    fin>>layerNo;
    cout<<"Layer Number: "<<layerNo<<endl;



    fin.seekg(1,ios::cur);
    std::getline(fin, line);
    fin.seekg(1,ios::cur);
    std::getline(fin, line);
    fin.seekg(1,ios::cur);
    std::getline(fin, line);

    fin>>chipL>>chipW>>asicTh>>sensorTh>>substrTh>>pitchR>>pitchC>>diodeOffsetX>>diodeOffsetY>>diodeSize>>rowNo>>colNo>>nAdcBit>>adcR>>_cce>>_enc>>_digiMethod;
    cout<<"L: "<<chipL<<" W: "<<chipW<<" asTh: "<<asicTh<<" senTh: "<<sensorTh<<" subTh: "<<substrTh<<" pitchR: "<<pitchR<<" pitchC: "<<pitchC<<" diodeOffsetX: "<<diodeOffsetX<<" diodeOffsetY: "<<diodeOffsetY<<" diodeSize: "<<diodeSize<<" rowNo: "<<rowNo<<" colNo: "<<colNo<<" ADCBitNo: "<<nAdcBit<<" ADCRange: "<<adcR<<" CCE: "<<_cce<<" Noise: "<<_enc<<endl;

    fin.seekg(1,ios::cur);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);

    fin>>ssW>>epoxyT>>kptT>>mfT>>cfT;
    cout<<ssW<<"\t"<<epoxyT<<"\t"<<kptT<<"\t"<<mfT<<"\t"<<cfT<<endl;

    fin.seekg(1,ios::cur);
    getline(fin, line);
    getline(fin, line);
    getline(fin, line);

    for(int i=0; i<layerNo; i++){
        fin>>name>>ladderNo>>ssL>>chipNo>>_R>>_Phi>>_PhaseAngle;
        cout<<name<<"\t"<<ladderNo<<"\t"<<ssL<<"\t"<<chipNo<<"\t"<<_R<<"\t"<<_Phi<<"\t"<<_PhaseAngle<<endl;
        getline(fin, line);


        //Consistent
        double activeSizeX=rowNo*pitchR *um;
        double phAngle=_PhaseAngle *deg;
        double chipR=_R *mm;
        double segmentSpanAngle = activeSizeX*cos(phAngle)/chipR;
        int nofLad=ceil(twopi/segmentSpanAngle);
        segmentSpanAngle = twopi/ladderNo;
        double conPhA=acos(segmentSpanAngle*chipR/activeSizeX)/deg;
        cout<<"Consistent Config::Least Ladder="<<nofLad<<"\t"<<"PhaseAngle="<<conPhA<<endl;


        //Chip
        fLayer[i].RowNo(rowNo);
        fLayer[i].ColNo(colNo);
        fLayer[i].ADCBitNo(nAdcBit);
        fLayer[i].ADCRange(adcR);
        fLayer[i].CCE(_cce);
        fLayer[i].ENC(_enc);
        fLayer[i].DigiMethod(_digiMethod);
        fLayer[i].PitchX(pitchC);  //*um
        fLayer[i].PitchY(pitchR);  //*um
        fLayer[i].DiodeOffsetX(diodeOffsetX);
        fLayer[i].DiodeOffsetY(diodeOffsetY);
        fLayer[i].DiodeSize(diodeSize);
        fLayer[i].ChipLength(chipL);   //*mm
        fLayer[i].ChipWidth(chipW);   //*mm
        fLayer[i].AsicThick(asicTh);     //*um
        fLayer[i].SensorThick(sensorTh);     //*um
        fLayer[i].SubstrateThick(substrTh);     //*um
        fLayer[i].R(_R);	      //*mm
        fLayer[i].Phi(_Phi);	      
        //fLayer[i].SetLayerName(name);
        //Ladder
        fLayer[i].ChipNo(chipNo);    
        fLayer[i].LadderLength(ssL);   //*mm
        fLayer[i].LadderWidth(ssW);   //*mm
        fLayer[i].EpoxyThick(epoxyT);    //*mm
        fLayer[i].KaptonThick(kptT);    //*mm
        fLayer[i].MetalFoilThick(mfT);    //*mm
        fLayer[i].CarbonFiberThick(cfT);    //*mm
        //Layer
        fLayer[i].LadderNo(ladderNo);
        fLayer[i].PhaseAngle(_PhaseAngle);	//*deg

    }

    fin.seekg(1,ios::cur);
    getline(fin, line);
    getline(fin, line);
    fin>>fSptSegmentNo;
    cout<<"SptSegment Number: "<<fSptSegmentNo<<endl;

    fin.seekg(1,ios::cur);
    getline(fin, line);
    getline(fin, line);

    for(int j=0; j<fSptSegmentNo; j++){
        fin>>name>>segInnR>>segOutR>>segL>>segZ;
        cout<<name<<"\t"<<segInnR<<"\t"<<segOutR<<"\t"<<segL<<"\t"<<segZ<<endl;
        getline(fin, line);

        fSptSegment[j].InnerR(segInnR);
        fSptSegment[j].OutR(segOutR);
        fSptSegment[j].Length(segL);
        fSptSegment[j].Z(segZ);
    }

    fin.close();

}

const JadePixLayer& JadePixGeoParameter::Layer(int layerId) const {
    if(layerId<0 || layerId>=layerNo){
        cout<<"JadePixGeoParameter::Error: Wrong layer No"<<endl;
    }
    return fLayer[layerId];
}

void JadePixGeoParameter::Dump(void){
    cout<<""<<endl;
    cout<<"***************** JadePixGeoParameter Information *******************"<<endl;
    cout<<"Layer No: "<<layerNo<<endl;
    for(int i=0;i<layerNo;++i){
        cout<<"Layer["<<i<<"]: "
            //	<<"Name: "<<fLayer[i].LayerName()
            <<"! Ladder Number: "<<fLayer[i].LadderNo()
            <<"! Chip Number: "<<fLayer[i].LayerChipNo()
            <<"! Pixel Number: "<<fLayer[i].LayerPixNo()<<endl;
        cout<<""<<endl;
    }
    cout<<"*******************************************************************"<<endl;
}
