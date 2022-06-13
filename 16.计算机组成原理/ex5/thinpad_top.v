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

// disable uart
assign uart_rdn = 1'b1;
assign uart_wrn = 1'b1;

wire clk;
assign clk = clk_11M0592;

// SRAM
reg oe_sram_n, we_sram_n;
reg[31:0] data_sram_in;
wire[31:0] data_sram_out;
wire sram_done;

sram_io _sram_io(
            .clk(clk),
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

reg[21:0] pc;   // program counter
reg[31:0] inst; // instruction
reg[31:0] register[0:31];
localparam REG_ZERO = 5'b0;

wire[31:0] reg_rs1, reg_rs2;
wire[4:0] rd;     // to judge if rd == REG_ZERO
assign reg_rs1 = register[inst[19:15]];
assign reg_rs2 = register[inst[24:20]];
assign rd = inst[11:7];

reg[11:0] imm;  // 12 bit 立即数
wire[19:0] sign_extend;
wire[31:0] imm_extend;  // 32 bit 符号扩展
assign sign_extend = imm[11] ? 20'hfffff : 20'h0;
assign imm_extend = {sign_extend, imm};

wire[6:0] opcode;
assign opcode = inst[6:0];
localparam OP_ADD = 7'b0110011; // R型 func7 rs2  rs1 func3 rd opcode
localparam OP_ORI = 7'b0010011; // I型 imm[11:0]   rs1 func3 rd opcode
localparam OP_LW = 7'b0000011;  // I型
localparam OP_SW = 7'b0100011;  // S型 imm[11:5] rs2 rs1 func3 imm[4:0] opcode
localparam OP_BEQ = 7'b1100011; // B型 imm[12|10:5] rs2 rs1 func3 imm[4:1|11] opcode

reg[31:0] alu_res;  // alu 计算结果

reg[2:0] state;
localparam STATE_IDLE = 3'b111; // 空状态
localparam STATE_IF_BEGIN = 3'b000; // 取指 fecth
localparam STATE_IF = 3'b001;
localparam STATE_ID = 3'b010;   // 译码 decode
localparam STATE_EXE = 3'b011;  // 执行 exec
localparam STATE_MEM_BEGIN = 3'b100;    // 访存
localparam STATE_MEM = 3'b101;
localparam STATE_WB = 3'b110;   // 写回

reg[19:0] addr;
assign base_ram_addr = addr;

always @(posedge clk or posedge reset_btn) begin
    if (reset_btn) begin
        state <= STATE_IF_BEGIN;
        {oe_sram_n, we_sram_n} <= 2'b11;
        register[REG_ZERO] <= 32'b0;
        {pc, inst} <= 64'b0;
    end
    else begin
        case (state)
            STATE_IF_BEGIN: begin
                state <= STATE_IF;
                oe_sram_n <= 1'b0;
                addr <= pc[21:2];
            end
            STATE_IF: begin
                if (sram_done) begin
                    state <= STATE_ID;
                    oe_sram_n <= 1'b1;
                    inst <= data_sram_out;
                end
            end
            STATE_ID: begin // 处理imm
                case (opcode)
                    OP_ADD: begin
                        imm <= 0;
                    end
                    OP_ORI, OP_LW: begin
                        imm <= inst[31:20];
                    end
                    OP_BEQ: begin
                        imm <= {inst[31], inst[7], inst[30:25], inst[11:8], 1'b0};
                    end
                    OP_SW: begin
                        imm <= {inst[31:25], inst[11:7]};
                    end
                    default: begin
                        imm <= 0;
                        state <= STATE_IDLE;
                    end
                endcase
                state <= STATE_EXE;
            end
            STATE_EXE: begin
                if (opcode == OP_BEQ) begin
                    if (reg_rs1 == reg_rs2) begin   // 跳转
                        pc <= pc + imm_extend;
                    end
                    else begin
                        pc <= pc + 4;
                    end
                    state <= STATE_IF_BEGIN;  // 进入下个周期
                end
                else begin
                    case (opcode)
                        OP_ADD: begin
                            alu_res <= reg_rs1 + reg_rs2;
                            state <= STATE_WB;
                        end
                        OP_ORI: begin
                            alu_res <= reg_rs1 | imm_extend;
                            state <= STATE_WB;
                        end
                        OP_LW, OP_SW: begin
                            alu_res <= reg_rs1 + imm_extend;
                            state <= STATE_MEM_BEGIN;
                        end
                        default: begin
                            alu_res <= 0;
                            state <= STATE_IDLE;
                        end
                    endcase
                    pc <= pc + 4;
                end
            end
            STATE_MEM_BEGIN: begin
                case (opcode)
                    OP_LW: begin
                        oe_sram_n <= 1'b0;
                    end
                    OP_SW: begin
                        we_sram_n <= 1'b0;
                        data_sram_in <= reg_rs2;
                    end
                endcase
                addr <= alu_res[21:2];
                state <= STATE_MEM;
            end
            STATE_MEM: begin
                if (sram_done) begin
                    {oe_sram_n, we_sram_n} <= 2'b11;
                    case (opcode)
                        OP_LW: begin
                            alu_res <= data_sram_out;
                            state <= STATE_WB;
                        end
                        OP_SW: begin
                            state <= STATE_IF_BEGIN; // 进入下个周期
                        end
                    endcase
                end
            end
            STATE_WB: begin
                if (rd == REG_ZERO) begin
                    state <= STATE_IDLE;
                end
                else begin
                    register[rd] <= alu_res;
                    state <= STATE_IF_BEGIN;
                end
            end
        endcase
    end
end
endmodule
