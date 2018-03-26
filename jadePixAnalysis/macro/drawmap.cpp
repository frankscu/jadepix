void drawmap(std::string file_name)
{

    
    std::cout << "Process file: " << file_name << std::endl;
    
    TFile *infile = new TFile(file_name.c_str());

    auto outfile_name = file_name.substr(0,file_name.find(".")) + "_map.root";

    TFile *outfile = new TFile(outfile_name.c_str(), "RECREATE");
    if (!infile->IsOpen())
    {
        std::cerr << "Cannot open file!" << std::endl;
        return;
    }


    const size_t NCOLS = 16;
    const size_t NROWS = 48;


    TH2F* mean_map = new TH2F("hist_mean","hist_mean", 16, 1, 16, 48, 1, 48);
    TH2F* rms_map = new TH2F("hist_rms","hist_rms", 16, 1, 16, 48, 1, 48);

    for (size_t ch = 0; ch < NCOLS; ch++)
        for (size_t row = 0; row < NROWS; row++)
        {
            std::stringstream hist_title_adc_dcs;
            hist_title_adc_dcs << "h_adc_dcs_ch" << (ch+1) << "_row" << (row+1);
            
            auto hist = (TH1F*)infile->Get(hist_title_adc_dcs.str().c_str());
            double mean = hist->GetMean();
            double rms = hist->GetRMS();
            mean_map->Fill(ch,row,mean);
            rms_map->Fill(ch,row,rms);
            //std::cout << "mean" << mean << "; rms "<< rms << std::endl;
        }
    
    TCanvas* c1 = new TCanvas();
    c1->Divide(2,1);
    c1->cd(1);
    mean_map->Draw("colz");
    c1->cd(2);
    rms_map->Draw("colz");
    c1->Write();
    outfile->Close();
}