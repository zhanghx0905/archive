`default_nettype none

module thinpad_top(
           input wire clk_50M,           //50MHz 时钟输入
           input wire clk_11M0592,       //11.0592MHz 时钟输入（备用，可不用）

           input wire clock_btn,         //BTN5手动时钟按钮开关，带消抖电路，按下时为1
           input wire reset_btn,         //BTN6手动复位按钮开关，带消抖电路，按下时为1

           input  wire[3:0]  touch_btn,  //BTN1~BTN4，按钮开关，按下时为1
           input  wire[31:0] dip_sw,     //32位拨码开关，拨到“ON”时为1
           output wire[15:0] leds,       //16位LED，输出时1点亮
           output wire[7:0]  dpy0,       //数码管低位信号，包括小数点，输出1点亮
           output wire[7:0]  dpy1,       //数码管高位信号，包括小数点，输出1点亮

           //CPLD串口控制器信号
           output wire uart_rdn,         //读串口信号，低有效
           output wire uart_wrn,         //写串口信号，低有效
           input wire uart_dataready,    //串口数据准备好
           input wire uart_tbre,         //发送数据标志
           input wire uart_tsre,         //数据发送完毕标志

           //BaseRAM信号
           inout wire[31:0] base_ram_data,  //BaseRAM数据，低8位与CPLD串口控制器共享
           output wire[19:0] base_ram_addr, //BaseRAM地址
           output wire[3:0] base_ram_be_n,  //BaseRAM字节使能，低有效。如果不使用字节使能，请保持为0
           output wire base_ram_ce_n,       //BaseRAM片选，低有效
           output wire base_ram_oe_n,       //BaseRAM读使能，低有效
           output wire base_ram_we_n,       //BaseRAM写使能，低有效

           //ExtRAM信号
           inout wire[31:0] ext_ram_data,  //ExtRAM数据
           output wire[19:0] ext_ram_addr, //ExtRAM地址
           output wire[3:0] ext_ram_be_n,  //ExtRAM字节使能，低有效。如果不使用字节使能，请保持为0
           output wire ext_ram_ce_n,       //ExtRAM片选，低有效
           output wire ext_ram_oe_n,       //ExtRAM读使能，低有效
           output wire ext_ram_we_n,       //ExtRAM写使能，低有效

           //直连串口信号
           output wire txd,  //直连串口发送端
           input  wire rxd,  //直连串口接收端

           //Flash存储器信号，参考 JS28F640 芯片手册
           output wire [22:0]flash_a,      //Flash地址，a0仅在8bit模式有效，16bit模式无意义
           inout  wire [15:0]flash_d,      //Flash数据
           output wire flash_rp_n,         //Flash复位信号，低有效
           output wire flash_vpen,         //Flash写保护信号，低电平时不能擦除、烧写
           output wire flash_ce_n,         //Flash片选信号，低有效
           output wire flash_oe_n,         //Flash读使能信号，低有效
           output wire flash_we_n,         //Flash写使能信号，低有效
           output wire flash_byte_n,       //Flash 8bit模式选择，低有效。在使用flash的16位模式时请设为1

           //USB 控制器信号，参考 SL811 芯片手册
           output wire sl811_a0,
           //inout  wire[7:0] sl811_d,     //USB数据线与网络控制器的dm9k_sd[7:0]共享
           output wire sl811_wr_n,
           output wire sl811_rd_n,
           output wire sl811_cs_n,
           output wire sl811_rst_n,
           output wire sl811_dack_n,
           input  wire sl811_intrq,
           input  wire sl811_drq_n,

           //网络控制器信号，参考 DM9000A 芯片手册
           output wire dm9k_cmd,
           inout  wire[15:0] dm9k_sd,
           output wire dm9k_iow_n,
           output wire dm9k_ior_n,
           output wire dm9k_cs_n,
           output wire dm9k_pwrst_n,
           input  wire dm9k_int,

           //图像输出信号
           output wire[2:0] video_red,    //红色像素，3位
           output wire[2:0] video_green,  //绿色像素，3位
           output wire[1:0] video_blue,   //蓝色像素，2位
           output wire video_hsync,       //行同步（水平同步）信号
           output wire video_vsync,       //场同步（垂直同步）信号
           output wire video_clk,         //像素时钟输出
           output wire video_de           //行数据有效信号，用于区分消隐区
       );

//disable byte
assign base_ram_be_n = 4'b0000;
assign ext_ram_be_n = 4'b0000;

//disable ext ram
assign ext_ram_ce_n = 1'b1;
assign ext_ram_oe_n = 1'b1;
assign ext_ram_we_n = 1'b1;

reg oe_sram_n, we_sram_n;
reg[31:0] data_sram_in;
wire[31:0] data_sram_out;
wire sram_done;

reg oe_uart_n, we_uart_n;
reg[7:0] data_uart_in;
wire[7:0] data_uart_out;
wire uart_done;

sram_io _sram_io(
            .clk(clk_50M),
            .rst(reset_btn),
            .oen(oe_sram_n),
            .wen(we_sram_n),
            .data_in(data_sram_in),
            .data_out(data_sram_out),
            .done(sram_done),

            .base_ram_data_wire(base_ram_data),

            .base_ram_ce_n(base_ram_ce_n),
            .base_ram_oe_n(base_ram_oe_n),
            .base_ram_we_n(base_ram_we_n)
        );


uart_io _uart_io(
            .clk(clk_50M),
            .rst(reset_btn),
            .oen(oe_uart_n),
            .wen(we_uart_n),
            .data_in(data_uart_in),
            .data_out(data_uart_out),
            .done(uart_done),
            .base_ram_data_wire(base_ram_data),
            .uart_rdn(uart_rdn),
            .uart_wrn(uart_wrn),
            .uart_dataready(uart_dataready),
            .uart_tbre(uart_tbre),
            .uart_tsre(uart_tsre)
        );

reg[3:0] count;
localparam MAX_COUNT = 4'hA;

reg[3:0] state;
localparam UART_READ_BEGIN  = 4'b0001;
localparam UART_READ    = 4'b0010;
localparam SRAM_WRITE_BEGIN = 4'b0011;
localparam SRAM_WRITE   = 4'b0100;
localparam SRAM_READ_BEGIN  = 4'b0101;
localparam SRAM_READ    = 4'b0110;
localparam UART_WRITE_BEGIN = 4'b0111;
localparam UART_WRITE   = 4'b1000;
localparam STATE_DONE = 4'b1111;
SEG7_LUT _SEG7_LUT_1(dpy1, state);

reg[19:0] addr;
assign base_ram_addr = addr;


always @(posedge clk_50M or posedge reset_btn) begin
    if (reset_btn) begin
        {oe_uart_n, we_uart_n} <= 2'b11;
        {oe_sram_n, we_sram_n} <= 2'b11;
        state <= UART_READ_BEGIN;
        addr <= dip_sw[19:0];
        count <= 4'h0;
    end
    else begin
        case (state)
            // Stage 1
            UART_READ_BEGIN: begin
                if (count == MAX_COUNT) begin
                    state <= SRAM_READ_BEGIN;
                    addr <= addr - MAX_COUNT;
                    count <= 4'h0;
                end
                else if (uart_dataready) begin
                    oe_uart_n <= 1'b0;
                    state <= UART_READ;
                end
            end
            UART_READ: begin
                if (uart_done) begin
                    oe_uart_n <= 1'b1;
                    state <= SRAM_WRITE_BEGIN;
                    data_sram_in <= {24'b0, data_uart_out};
                end
            end
            SRAM_WRITE_BEGIN: begin
                we_sram_n <= 1'b0;
                state <= SRAM_WRITE;
            end
            SRAM_WRITE: begin
                if (sram_done) begin
                    we_sram_n <= 1'b1;
                    state <= UART_READ_BEGIN;
                    addr <= addr + 1'b1;
                    count <= count + 1'b1;
                end
            end
            // Stage 2
            SRAM_READ_BEGIN: begin
                if (count == MAX_COUNT) begin
                    state <= STATE_DONE;
                    addr <= addr - MAX_COUNT;
                    count <= 4'h0;
                end
                else begin
                    oe_sram_n <= 1'b0;
                    state <= SRAM_READ;
                end
            end
            SRAM_READ: begin
                if (sram_done) begin
                    oe_sram_n <= 1'b1;
                    state <= UART_WRITE_BEGIN;
                    data_uart_in <= data_sram_out[7:0];
                end
            end
            UART_WRITE_BEGIN: begin
                we_uart_n <= 1'b0;
                state <= UART_WRITE;
            end
            UART_WRITE: begin
                if (uart_done) begin
                    we_uart_n <= 1'b1;
                    state <= SRAM_READ_BEGIN;
                    addr <= addr + 1'b1;
                    count <= count + 1'b1;
                end
            end
            // Do nothing
            STATE_DONE: begin
                state <= STATE_DONE;
            end
        endcase
    end
end

endmodule
