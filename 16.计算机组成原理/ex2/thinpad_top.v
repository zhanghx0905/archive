`default_nettype none

module thinpad_top(
    input wire clk_50M,           //50MHz 时钟输入
    input wire clk_11M0592,       //11.0592MHz 时钟输入（备用，可不用）

    input wire clock_btn,         //BTN5手动时钟按钮开关，带消抖电路，按下时为1
    input wire reset_btn,         //BTN6手动复位按钮开关，带消抖电路，按下时为1

    input  wire[3:0]  touch_btn,  //BTN1~BTN4，按钮开关，按下时为1
    input  wire[31:0] dip_sw,     //32位拨码开关，拨到"ON"时为1
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

// 数码管连接关系示意图，dpy1同理
// p=dpy0[0] // ---a---
// c=dpy0[1] // |     |
// d=dpy0[2] // f     b
// e=dpy0[3] // |     |
// b=dpy0[4] // ---g---
// a=dpy0[5] // |     |
// f=dpy0[6] // e     c
// g=dpy0[7] // |     |
//           // ---d---  p

// 7段数码管译码器演示，将number用16进制显示在数码管上面
reg[7:0] number;
SEG7_LUT segL(.oSEG1(dpy0), .iDIG(number[3:0])); //dpy0是低位数码管
SEG7_LUT segH(.oSEG1(dpy1), .iDIG(number[7:4])); //dpy1是高位数码管

/*
本示例代码已经实现了加法和左移。同学们需要补充完整剩下的代码。
主要需要补充工作包括：
1. 增加功能码;
2. 完成所有的功能操作;
3. 在状态机的不同状态做相应的动作。
注意：本代码虽然可以综合，但是仍然不可以执行，需要至少完成
上面3的状态机才能够演示基本的alu加法和左移功能。
*/

reg[15:0] led_bits;
assign leds = led_bits;

//有限状态机状态码
reg[1:0] state;
parameter S0 = 2'b00;
parameter S1 = 2'b01;
parameter S2 = 2'b10;
parameter S3 = 2'b11;

parameter Add = 4'b0001;
parameter Sll = 4'b0111;
// TODO begin
parameter Sub = 4'b0010;
parameter And = 4'b0011;
parameter Or = 4'b0100;
parameter Xor = 4'b0101;
parameter Not = 4'b0110;
parameter Srl = 4'b1000;
parameter Sra = 4'b1001;
parameter Rol = 4'b1010; 
/* TODO end
增加下列的功能码： Sub, And, Or, Xor, Not, Sll, Srl, Sra, Rol.
*/

reg[15:0] a;
reg[15:0] b;
reg[3:0] op;
reg[15:0] result;
reg[3:0] flags;
// Cache local variables
reg[15:0] tmp16;
reg[31:0] tmp32;

//四个标志位
reg cf = 1'b0; //Carry Flag 
reg zf = 1'b0; //Zero Flag 
reg sf = 1'b0; //Signed Flag 
reg vf = 1'b0; //Overflow Flag

//不同状态下LED显示的内容不一样
always@* begin
    case(state)
        S0: begin
            led_bits=dip_sw;
        end
        S1: begin
            led_bits=dip_sw;
        end
        S2: begin
            led_bits=result;
        end
        S3: begin
            led_bits=flags;
        end
        default:begin
            led_bits = 16'hFFFF;
        end
    endcase
end

always@* begin
    op=dip_sw[3:0];
    result=0;
    zf=0;
    cf=0;
    sf=0;
    vf=0;
    
    case(op)
        Add:begin
            {cf, result} = a + b;
            vf = (~a[15] && ~b[15] && result[15]) || (a[15] && b[15] && ~result[15]);
            // TODO: 更新标志位
        end
        Sll:begin
            result = a << b;
        end
        Sub:begin
            {cf, result} = a - b;
            vf = (~a[15] && b[15] && result[15]) || (a[15] && ~b[15] && ~result[15]);
        end
        And:begin
            result = a & b;
        end
        Or:begin
            result = a | b;
        end
        Xor:begin
            result = a ^ b;
        end
        Not:begin
            result = ~a;
        end
        Srl:begin
            result = a >> b;
        end
        Sra:begin
            tmp16 = (a[15] == 0) ? 16'b0 : 16'hffff;
            tmp32 = {tmp16, a} >> b;
            result = tmp32[15:0];
        end
        Rol:begin
            tmp32 = {a, a} << b;
            result = tmp32[31:16];
        end
        //TODO:
        // 分别支持 Sub, And, Or, Xor, Not, Sll, Srl, Rol. 指令
        // 并在合适的位置计算zf、sf、vf、cf
    endcase
    zf = result == 0? 1'b1: 1'b0;
end

always@(posedge clock_btn or posedge reset_btn) begin
    if (reset_btn) begin //复位键
        number = 0;
        state = 0;
    end
    else begin //运行状态机
        case (state)
            S0:begin
                a = dip_sw;
                flags = 0;
                number = 1;
            end
            S1:begin
               b = dip_sw;
               number = 2; 
            end
            S2:begin
                flags = {12'b0, cf, zf, sf, vf};
                number = 3;
            end
            S3:begin
                number = 4;
            end
            // TODO:
            // S1: 输入b
            // S2: 显示结果
            // S3: 显示cf,zf,sf,vf
        endcase
        state = state + 1;
    end
end

endmodule
