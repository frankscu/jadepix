void drawmap(std::string file_name)
{

    std::cout << "Draw Map file: " << file_name << std::endl;
    TFile *infile = new TFile(file_name.c_str());
    if (!infile->IsOpen())
    {
        std::cerr << "Cannot open file!" << std::endl;
        return;
    }

    const size_t NCOLS = 48;
    const size_t NROWS = 16;


    for (size_t i = 0; i < NCOLS; i++)
        for (size_t j = 0; i < NROWS; j++)
        {
            auto hist = infile->Get(Form("h_adc_dcs_ch%s_row%s", i+1, j+1));
            auto mean = hist->GetMean();
            std::cout << "mean" << std::endl;
        }
}