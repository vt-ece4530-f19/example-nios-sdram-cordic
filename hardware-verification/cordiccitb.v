module cordiccitb;

   reg [31:0] dataa, datab;
   reg clk, clk_en;
   reg reset;
   reg start;
   reg [7:0] n;

   wire [31:0] result;
   wire done;

   cordicci DUT(.dataa(dataa),
                .datab(datab),
                .clk(clk),
                .clk_en(clk_en),
                .reset(reset),
                .start(start),
                .n(n),
                .result(result),
                .done(done));

   always begin
        clk = 1'b0;
        #10;
        clk = 1'b1;
        #10;
        end

  initial begin

     reset  = 1'b1;
     clk_en = 1'b1;
     start  = 1'b0;
     dataa  = 32'd0;
     datab  = 32'd0;
     n      = 8'd0;

     @(posedge   clk) ;
     @(posedge   clk) ;
     @(posedge   clk) ;

     reset = 1'b0;

     n      = 8'd0;
     dataa  = 32'd16000;
     datab  = 32'd0;
 
     start = 1'b1;
     @(posedge clk);
     start = 1'b0;

     @(posedge done) ;

     #3;
     
     @(posedge   clk) ;
     @(posedge   clk) ;
     @(posedge   clk) ;
     
     n      = 8'd1;
     dataa  = 32'd0;
     datab  = 32'd0;
 
     start = 1'b1;
     @(posedge clk);
     start = 1'b0;

     @(posedge done) ;

     #3;
     
     @(posedge   clk) ;
     @(posedge   clk) ;
     @(posedge   clk) ;
     
     n      = 8'd2;
     dataa  = 32'd0;
     datab  = 32'd0;
 
     start = 1'b1;
     @(posedge clk);
     start = 1'b0;

     @(posedge done) ;

     #3;
     
     @(posedge   clk) ;
     @(posedge   clk) ;
     @(posedge   clk) ;
     
     $finish;
     
  end
   
endmodule
