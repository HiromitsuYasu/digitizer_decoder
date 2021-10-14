int make_rootfile(TString filename = "./measurement/20211014_17/20211014_17_0.raw"){

ifstream file;
file.open(filename,  ios::in | ios::binary);
TString outfilename = filename + ".root";

//check file open
if(file.is_open()) {
    cout << "file open " << filename << endl;    
} else {
    cout << "error No such " << filename << endl;
    return -1;
}

UShort_t wfs[8][1024];

//make tree
TFile *fout = new TFile(outfilename,"recreate");
TTree *tree = new TTree("tree","Digitizer data");

tree->Branch("wfs", wfs , "wfs[8][1024]/s");
UShort_t buf2b;

while(true){
    //read file and skip 0x5555 & 0xAAAA 
    file.read((char*) &buf2b, 2);
    //cout << hex << buf2b << endl;
    if(buf2b != 0x5555) break;

    file.read((char*) &buf2b, 2);
    //cout << hex << buf2b << endl;

    //read ch data
    for(int ch=0; ch< 8; ++ch){
        for(int j =0;j <1024;++j){
            file.read((char*) &buf2b, 2);
            // debug

            UShort_t value = ((0x00FF & buf2b)<<8) | ((0xFF00 & buf2b)>>8); // Yuto cant understand this magic
            wfs[ch][j] = value;
        }
    }   

    for(int i=0;i <= 8193;++i){
        file.read((char*) &buf2b, 2);   
    }
    tree->Fill();
}

fout->Write();
fout->Close();

return 0;
}