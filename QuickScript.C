{
  TLine *zer = new TLine(-2.5,0,2.5,0);
  zer->SetLineStyle(kDashed);

  TH2F *h1 = new TH2F("h1","MyFit",5,-2.5,2.5,5,-300,300);
  //TH2F *h2 = new TH2F("h2","GadeFit",5,-2.5,2.5,5,-50,50);
  
  TCanvas *C1 = new TCanvas("C1");
  C1->Divide(2);
  C1->cd(1);
  IMME_graph1->Draw();
  C1->cd(2);
  h1->Draw();
  IMME_Resid_graph1->Draw("P");
  zer->Draw();

  /*  C1->cd(2);
  h2->Draw();
  IMME_ResidGade_graph1->Draw("P");
  zer->Draw();
  */
}
