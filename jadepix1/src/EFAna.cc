#include <Math/DisplacementVector2D.h>
#include <Math/Vector2D.h>
#include <Math/Vector3D.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <memory>

// FIXME: these includes should be absolute and provided with installation?

#include "../../src/objects/MCParticle.hpp"
#include "../../src/objects/PixelCharge.hpp"
#include "../../src/objects/PixelHit.hpp"
#include "../../src/objects/PropagatedCharge.hpp"

/**
 * Construct a ROOT TTree from the data objects that can be used for comparison
 */
std::shared_ptr<TTree> constructComparisonTree(TFile* file, std::string dut, size_t cluster_size, std::pair<size_t, size_t>pix_size) {
  // Read pixel hit output
  TTree* pixel_hit_tree = static_cast<TTree*>(file->Get("PixelHit"));
  if(!pixel_hit_tree) {
    std::cout << "Could not read tree PixelHit, cannot continue." << std::endl;
    return std::make_shared<TTree>();
  }

  TBranch* pixel_hit_branch = pixel_hit_tree->FindBranch(dut.c_str());
  if(!pixel_hit_branch) {
    std::cout << "Could not find the DUT branch on tree PixelHit, cannot continue." << std::endl;
    return std::make_shared<TTree>();
  }

  std::vector<allpix::PixelHit*> input_hits;
  pixel_hit_branch->SetObject(&input_hits);

  // Read deposited and propagated charges for history
  TTree* deposited_charge_tree = static_cast<TTree*>(file->Get("DepositedCharge"));
  if(!deposited_charge_tree) {
    std::cout << "Could not read tree DepositedCharge" << std::endl;
  }
  TTree* propagated_charge_tree = static_cast<TTree*>(file->Get("PropagatedCharge"));
  if(!propagated_charge_tree) {
    std::cout << "Could not read tree PropagatedCharge" << std::endl;
  }

  // Read pixel charge output
  TTree* pixel_charge_tree = static_cast<TTree*>(file->Get("PixelCharge"));
  if(!pixel_charge_tree) {
    std::cout << "Could not read tree PixelCharge" << std::endl;
  }

  TBranch* pixel_charge_branch = pixel_charge_tree->FindBranch(dut.c_str());
  std::vector<allpix::PixelCharge*> input_charges;
  pixel_charge_branch->SetObject(&input_charges);

  // Read MC truth
  TTree* mc_particle_tree = static_cast<TTree*>(file->Get("MCParticle"));
  if(!mc_particle_tree) {
    std::cout << "Could not read tree MCParticle" << std::endl;
  }
  TBranch* mc_particle_branch = mc_particle_tree->FindBranch(dut.c_str());
  if(!mc_particle_branch) {
    std::cout << "Could not find the DUT branch on tree MCParticle" << std::endl;
  }

  std::vector<allpix::MCParticle*> input_particles;
  mc_particle_branch->SetObject(&input_particles);

  // Initialize output tree and branches
  auto output_tree = std::make_shared<TTree>("clusters", ("Cluster information for " + dut).c_str());
  int event_num;
  // Event number
  output_tree->Branch("eventNr", &event_num);
  // Cluster size
  int output_cluster, output_cluster_x, output_cluster_y;
  double aspect_ratio;
  output_tree->Branch("size", &output_cluster);
  output_tree->Branch("sizeX", &output_cluster_x);
  output_tree->Branch("sizeY", &output_cluster_y);
  output_tree->Branch("aspectRatio", &aspect_ratio);
  // Charge info
  int output_total_charge;
  std::vector<int> output_charge;
  output_tree->Branch("totalCharge", &output_total_charge);
  output_tree->Branch("charge", &output_charge);
  // Signal info
  int output_total_signal;
  const size_t ncols = pix_size.first;
  const size_t nrows = pix_size.second;
  long pixel_signal[ncols][nrows];
  std::vector<int> output_signal;
  std::vector<int> output_cent_signal;
  std::vector<int> output_seed_signal;
  std::vector<int> output_cent_clus_signal;
  std::vector<int> output_seed_clus_signal;
  output_tree->Branch("totalSignal", &output_total_signal);
  output_tree->Branch("signal", &output_signal);
  output_tree->Branch("center_signal", &output_cent_signal);
  output_tree->Branch("center_cluster_signal", &output_cent_clus_signal);
  output_tree->Branch("seed_signal", &output_seed_signal);
  output_tree->Branch("seed_cluster_signal", &output_seed_clus_signal);
  // Single pixel row / col
  std::vector<int> output_rows;
  std::vector<int> output_cols;
  output_tree->Branch("row", &output_rows);
  output_tree->Branch("col", &output_cols);
  // Real track information
  int output_track_count;
  double output_track_x, output_track_y;
  output_tree->Branch("trackCount", &output_track_count); // FIXME: problems arise if not one
  output_tree->Branch("trackLocalX", &output_track_x);
  output_tree->Branch("trackLocalY", &output_track_y);
  // Calculated track information and residuals
  double output_x, output_y, output_res_x, output_res_y;
  output_tree->Branch("localX", &output_x);
  output_tree->Branch("localY", &output_y);
  output_tree->Branch("resX", &output_res_x);
  output_tree->Branch("resY", &output_res_y);


  std::cout << "ncols, nrows: " << ncols << ", " << nrows << std::endl;
  std::cout << "cluster size: " << cluster_size << std::endl;
  const int pix_seed_th = 500;
  const int pix_clus_th = 0;

  // Convert tree for every event
  for(int i = 0; i < pixel_hit_tree->GetEntries(); ++i) {
    pixel_hit_tree->GetEntry(i);
    if(i % 5000 ==0){
      std::cout << "--------------> process " << i << " events --------------" << std::endl;
    }
    if(pixel_charge_tree)
      pixel_charge_tree->GetEntry(i);
    if(mc_particle_tree)
      mc_particle_tree->GetEntry(i);
    if(deposited_charge_tree)
      deposited_charge_tree->GetEntry(i);
    if(propagated_charge_tree)
      propagated_charge_tree->GetEntry(i);

    // Set event number
    event_num = i + 1;

    // Set cluster sizes
    output_cluster = input_hits.size();
    std::set<int> unique_x;
    std::set<int> unique_y;
    for(auto& hit : input_hits) {
      unique_x.insert(hit->getPixel().getIndex().x());
      unique_y.insert(hit->getPixel().getIndex().y());
    }
    output_cluster_x = unique_x.size();
    output_cluster_y = unique_y.size();
    aspect_ratio = static_cast<double>(output_cluster_y) / output_cluster_x;

    // Set charge information
    output_charge.clear();
    output_total_charge = 0;
    for(auto& pixel_charge : input_charges) {
      output_charge.push_back(pixel_charge->getCharge());
      output_total_charge += pixel_charge->getCharge();
    }

    // Set signal information
    output_signal.clear();
    output_total_signal = 0;
    for(auto& hit : input_hits) {
      output_signal.push_back(hit->getSignal());
      output_total_signal += hit->getSignal();
    }

    // Set pixel position information
    output_rows.clear();
    output_cols.clear();
    for(auto& hit : input_hits) {
      // FIXME defined order
      output_rows.push_back(hit->getPixel().getIndex().y());
      output_cols.push_back(hit->getPixel().getIndex().x());
    }

    // Initialize pixel array
    for(size_t i=0; i<ncols*nrows; i++){
      *((long*)pixel_signal+i)=0;
    }

    // Fill signal to every array
    for(auto& hit : input_hits) {
      // FIXME defined order
      auto _rows = hit->getPixel().getIndex().y();
      auto _cols = hit->getPixel().getIndex().x();
      pixel_signal[_cols][_rows] += hit->getSignal();
    }

    if(i % 5000 ==0) std::cout << "total pixel signal: " << output_total_signal << std::endl;

    output_cent_signal.clear();
    output_seed_signal.clear();
    output_cent_clus_signal.clear();
    output_seed_clus_signal.clear();

    if( output_total_signal > 0){
      long tmp_xs=0;
      long tmp_ys=0;
      size_t xc=0;
      size_t yc=0;
      size_t xd=0;
      size_t yd=0;

      long tmp_cent_pixel_signal=0;
      long tmp_seed_pixel_signal=0;
      long tmp_cent_clus_pixel_signal=0;
      long tmp_seed_clus_pixel_signal=0;


      for(size_t i=0; i<ncols; i++){
        for(size_t j=0; j<nrows; j++){

          if(pixel_signal[i][j] > pix_seed_th){

            bool seed_can_be_used = true;

            for(size_t ii=i-1; ii<=i+1; ii++)
              for(size_t jj=j-1; jj<=j+1; jj++){
                if((ii>0) && (ii<ncols) && (jj>0) && (jj<nrows)){
                  if(pixel_signal[ii][jj] > pixel_signal[i][j])
                  {
                    seed_can_be_used = false;
                  }
                }
              }

            if(seed_can_be_used){
              tmp_seed_pixel_signal = pixel_signal[i][j];
              xd = i;
              yd = j;
              output_seed_signal.push_back(tmp_seed_pixel_signal);
            }
          }

        }
      }


      for(size_t i=xd-cluster_size; i<=xd+cluster_size; i++)
        for(size_t j=yd-cluster_size; j<=yd+cluster_size; j++){
          if((i>0) && (i<ncols) && (j>0) && (j<nrows)){
            if(pixel_signal[i][j]>pix_clus_th){
              tmp_seed_clus_pixel_signal += pixel_signal[i][j];
            }
          }
        }

      if(tmp_seed_clus_pixel_signal > pix_clus_th){
        output_seed_clus_signal.push_back(tmp_seed_clus_pixel_signal);
      }

      for(size_t i=0; i<ncols; i++){
        long tmp_sum_x_pixel_signal=0;
        for(size_t j=0; j<nrows; j++){
          tmp_sum_x_pixel_signal += pixel_signal[i][j];
        }
        tmp_xs += i*tmp_sum_x_pixel_signal;
      }

      for(size_t j=0; j<nrows; j++){
        long tmp_sum_y_pixel_signal=0;
        for(size_t i=0; i<ncols; i++){
          tmp_sum_y_pixel_signal += pixel_signal[i][j];
        }
        tmp_ys += j*tmp_sum_y_pixel_signal;
      }

      if(output_total_signal>0){
        xc = static_cast<size_t>(std::round(tmp_xs/output_total_signal));
        yc = static_cast<size_t>(std::round(tmp_ys/output_total_signal));
      }


      if((xc>0) && (xc<ncols) && (yc>0) && (yc<nrows)){

        if(i % 5000 ==0) std::cout << "center pixel: " << xc << ", " << yc << std::endl;

        if(pixel_signal[xc][yc]>pix_seed_th){
          tmp_cent_pixel_signal=pixel_signal[xc][yc];
          output_cent_signal.push_back(tmp_cent_pixel_signal);

          if(i % 5000 ==0) std::cout << "center pixel signal: " << tmp_cent_pixel_signal << std::endl;

          for(size_t i=xc-cluster_size; i<=xc+cluster_size; i++)
            for(size_t j=yc-cluster_size; j<=yc+cluster_size; j++){
              if((i>0) && (i<ncols) && (j>0) && (j<nrows)){
                if(pixel_signal[i][j]>pix_clus_th){
                  tmp_cent_clus_pixel_signal += pixel_signal[i][j];
                }
              }
            }
          output_cent_clus_signal.push_back(tmp_cent_clus_pixel_signal);

        }
      }
    }

    // Get information about the actual track
    output_track_count = 1 + input_particles.size();
    output_track_x = 0;
    output_track_y = 0;

    // FIXME: guess the truth position from the average of start and end points
    for(auto& particle : input_particles) {
      output_track_x += (particle->getLocalStartPoint().x() + particle->getLocalEndPoint().x()) / 2.0;
      output_track_y += (particle->getLocalStartPoint().y() + particle->getLocalEndPoint().y()) / 2.0;
    }
    if(!input_particles.empty()) {
      output_track_x /= input_particles.size();
      output_track_y /= input_particles.size();
    }

    // Calculate local x using a simple center of gravity fit
    // FIXME no corrections are applied
    ROOT::Math::XYZVector totalPixel;
    double totalSignal = 0;
    for(auto& hit : input_hits) {
      totalPixel += static_cast<ROOT::Math::XYZVector>(hit->getPixel().getLocalCenter()) * hit->getSignal();
      totalSignal += hit->getSignal();
    }
    totalPixel /= totalSignal;
    output_x = totalPixel.x();
    output_y = totalPixel.y();
    output_res_x = output_track_x - output_x;
    output_res_y = output_track_y - output_y;

    output_tree->Fill();
  }

  return output_tree;
}


void print_usage(){
  printf("NAME\n\tana - Gen Allpix root files\n");
  printf("\nSYNOPSIS\n\tana input output\n ");

}

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage() ;
    return -1;
  }

  std::string opt_infile = "output/jadepix-user-test.root";
  std::string opt_outfile = "output/jadepix-ana-test.root";
  std::string opt_clustersize = "1";
  std::string opt_x = "48";
  std::string opt_y = "16";

  for(int i=1;  i<argc; i++){
    std::string opt(argv[i]);
    if(opt=="-f"){
      if(i+1<argc){
        i++;
        opt_infile = argv[i];
      }
    }
    if(opt=="-o"){
      if(i+1<argc){
        i++;
        opt_outfile = argv[i];
      }
    }
    if(opt=="-s"){
      if(i+1<argc){
        i++;
        opt_clustersize = argv[i];
      }
    }
    if(opt=="-x"){
      if(i+1<argc){
        i++;
        opt_x = argv[i];
      }
    }
    if(opt=="-y"){
      if(i+1<argc){
        i++;
        opt_y = argv[i];
      }
    }
  }

  TString inputFile = opt_infile;
  TString outputFile = opt_outfile;
  size_t cluster_size = static_cast<int>(std::stoul(opt_clustersize));
  size_t ncol = static_cast<int>(std::stoul(opt_x));
  size_t nrow = static_cast<int>(std::stoul(opt_y));

  std::pair<size_t, size_t> pix_size(ncol, nrow);

  printf("Input: %s\n", inputFile.Data());


  auto fin = new TFile(inputFile);

  auto fout = new TFile(outputFile, "RECREATE");

  auto tree = constructComparisonTree(fin, "dut", cluster_size, pix_size);
  tree->Write();
  fout->Close();

  printf("File saved as: %s\n", outputFile.Data());

}

