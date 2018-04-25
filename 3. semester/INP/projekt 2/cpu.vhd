-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2015 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Maros Cocula (xcocul00)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu
-- ----------------------------------------------------------------------------
      -- Signaly pre Multiplexory
  signal SEL1 : std_logic ; 
  signal SEL2 : std_logic_vector (1 downto 0); 
 
-- ----------------------------------------------------------------------------
      -- Signaly pre CNT
  signal CNT : std_logic_vector (7 downto 0);
  signal CNT_Set : std_logic;
  signal CNT_Inc : std_logic;
  signal CNT_Dec : std_logic;

  -- ----------------------------------------------------------------------------
      -- Signaly pre PC
  signal PC : std_logic_vector (12 downto 0);
  signal PC_Set : std_logic;
  signal PC_Inc : std_logic;
  signal PC_Dec : std_logic;

  -- ----------------------------------------------------------------------------
      -- Signaly pre PTR
  signal PTR : std_logic_vector (12 downto 0);
  signal PTR_Set : std_logic;
  signal PTR_Inc : std_logic;
  signal PTR_Dec : std_logic;
 
 -- ----------------------------------------------------------------------------
      -- Signaly pre TMP
   signal TMP : std_logic_vector (7 downto 0);
   signal TMP_Set : std_logic;

 -- ---------------------------------------------------------------------------- 
      -- Instrukcie
  type instructions is (I_START,I_INC, I_DEC, I_GO_LEFT, I_GO_RIGHT, I_LOOP_START, I_LOOP_END, I_OUTPUT, I_INPUT, I_SAVE_TMP, I_SAVE, I_RET);
  signal instructions_pointer : instructions;

-- ----------------------------------------------------------------------------
    -- CPU_status
    -- , S_GO_LEFT, S_GO_RIGHT
  type CPU_status is (S_START,S_LOAD_PREPARE, S_LOAD, S_STOP, S_INC, S_DEC, S_OUTPUT,S_INPUT,S_SAVE_TMP,S_SAVE,S_LOOP1,S_LOOP2,S_LOOP3,S_LOOP4, S_LOOP_END1, S_LOOP_END2, S_LOOP_END3, S_LOOP_END4);
  signal CPU_present : CPU_status;
  signal CPU_next : CPU_status;


begin
  
-- ----------------------------------------------------------------
  Load_instruction: process (DATA_RDATA)
  begin
    case DATA_RDATA is
    
      when X"3E" =>
        instructions_pointer <= I_GO_RIGHT;
      when X"3C" =>
        instructions_pointer <= I_GO_LEFT;
      when X"2B" =>
        instructions_pointer <= I_INC;
      when X"2D" =>
        instructions_pointer <= I_DEC;
      when X"5B" =>
        instructions_pointer <= I_LOOP_START;
      when X"5D" =>
        instructions_pointer <= I_LOOP_END;
      when X"2E" =>
        instructions_pointer <= I_OUTPUT;
      when X"2C" =>
        instructions_pointer <= I_INPUT;
      when X"24" =>
        instructions_pointer <= I_SAVE_TMP;
      when X"21" =>
        instructions_pointer <= I_SAVE;
      when X"00" =>
         instructions_pointer <= I_RET;
      when others =>
        instructions_pointer <= I_START;
    end case;
  end process;

-- ---------------------------------------------------------------------------- 
 process(SEL1,PC,PTR)
begin
   case SEL1 is
      when '0' => DATA_ADDR <= PC;
      when '1' => DATA_ADDR <= PTR;
      when others => 
   end case;
end process;
-- ---------------------------------------------------------------------------- 
process (SEL2, IN_DATA, DATA_RDATA, TMP)
   begin
      case SEL2 is
         when "00" => DATA_WDATA <= IN_DATA;
         when "01" => DATA_WDATA <= TMP;
         when "10" => DATA_WDATA <= DATA_RDATA-1;
         when "11" => DATA_WDATA <= DATA_RDATA+1;
         when others => 
      end case;
end process;
-- ---------------------------------------------------------------------------- 
    -- Proces pre CNT pocitadlo

  CNT_P: process (RESET, CLK)
  begin
    if (RESET = '1') then
      CNT <= "00000000";
    elsif ((CLK'event) and (CLK = '1') and (EN='1')) then
      if (CNT_Inc = '1') then
         CNT <= CNT+1;
      elsif (CNT_Dec = '1') then
        CNT <= CNT-1;
      elsif (CNT_Set = '1') then
        CNT <= "00000001";
      end if;
    end if;
  end process;

     -- Proces pro pocitadlo pc

  PC_P: process (RESET, CLK)
  begin
    if (RESET = '1') then
      PC <= "0000000000000";
    elsif ((CLK'event) and (CLK = '1') and (EN='1')) then
      if (PC_Inc = '1') then
        PC <= PC+1;
      elsif (PC_Dec = '1') then
        PC <= PC-1;
      end if;
    end if;
  end process; 


    -- Proces pre PTR pocitadlo

  PTR_P: process (RESET, CLK)
  begin
    if (RESET = '1') then
      PTR <= "1000000000000";
    elsif ((CLK'event) and (CLK = '1') and (EN='1')) then
      if (PTR_Inc = '1') then
        PTR <= PTR+1;
      elsif (PTR_Dec = '1') then
        PTR <= PTR-1;
      end if;
    end if;
  end process;

TMP_P: process (RESET, CLK)
  begin
    if (RESET = '1') then
      TMP <= "00000000";
    elsif ((CLK'event) and (CLK = '1') and (EN='1')) then
      if (TMP_Set = '1') then
        TMP <= DATA_RDATA;
    end if;
   end if;
  end process;

    -- Predosli stav
  CPU_present_P: process (RESET, CLK, EN)
  begin
    if (RESET = '1') then
      CPU_present <= S_START;
    elsif ((CLK'event) and (CLK = '1')) then
      if (EN = '1') then
        CPU_present <= CPU_next;
      end if;
    end if;
  end process;

   -- Dalsi stav  FSM
CPU_next_P: process ( EN, IN_DATA, IN_VLD, OUT_BUSY,  PTR, CNT, CPU_present, instructions_pointer , DATA_RDATA )
   begin


      OUT_WE <= '0';
      IN_REQ <= '0';
      DATA_RDWR <= '0';
      DATA_EN <= '0';

      PTR_Set <= '0';
      PTR_Inc <= '0';
      PTR_Dec <= '0';
      PC_Set <= '0';
      PC_Inc <= '0';
      PC_Dec <= '0';
      CNT_Set <= '0';
      CNT_Inc <= '0';
      CNT_Dec <= '0';
      TMP_Set <= '0';




      case CPU_present is
    
      when S_START =>
        CPU_next <= S_LOAD_PREPARE;
      when S_STOP =>
        CPU_next <= S_STOP;
      
      when S_LOAD_PREPARE =>
        DATA_EN <= '1';
        SEL1 <= '0';
        DATA_RDWR <= '1';
        CPU_next <= S_LOAD;
      
      when S_LOAD =>
        
         case instructions_pointer is
         
          when I_INC =>
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            SEL1 <= '1'; 
            CPU_next <= S_INC;
          
          when I_DEC =>
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            SEL1 <= '1';
            CPU_next <= S_DEC;
          
          when I_GO_LEFT =>
            PTR_Dec <= '1';
            PC_Inc <= '1';
            CPU_next <= S_START;
          
          when I_GO_RIGHT =>
            PTR_Inc <= '1';
            PC_Inc <= '1';
            CPU_next <= S_START;
          
          when I_OUTPUT =>
            DATA_EN <= '1';
            DATA_RDWR <= '1';           
            SEL1 <= '1';
            CPU_next <= S_OUTPUT;

          when I_INPUT =>
            IN_REQ <= '1';
            CPU_next <= S_INPUT;
          
          when I_RET =>
            CPU_next <= S_STOP;

          when I_START =>
            CPU_next <= S_START;

          when I_SAVE_TMP =>
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            SEL1 <= '1';
            CPU_next <= S_SAVE_TMP;
          
          when I_SAVE =>
            CPU_next <= S_SAVE;

          when I_LOOP_START =>
            CPU_next <= S_LOOP1; 

          when I_LOOP_END =>
            CPU_next <= S_LOOP_END1;
          
          when others =>
            PC_Inc <= '1';
            CPU_next <= S_START;            
        end case;
      
      when S_INC => 
        DATA_EN <= '1';
        SEL2 <= "11";
        DATA_RDWR <= '0';
        SEL1 <= '1';
        PC_Inc <= '1';
        CPU_next <= S_START;

      when S_DEC =>
        DATA_EN <= '1';
        SEL2 <= "10";
        SEL1 <= '1';
        DATA_RDWR <= '0';
        PC_Inc <= '1';
        CPU_next <= S_START;
      
      when S_OUTPUT =>
          if (OUT_BUSY = '1') then
              CPU_next <= S_OUTPUT;
          else
          OUT_WE <= '1';
          OUT_DATA <= DATA_RDATA;
          PC_Inc <= '1';
          CPU_next <= S_START;
          end if;

      when S_INPUT =>
        if(IN_VLD = '0') then
          IN_REQ <= '1';
          CPU_next <= S_INPUT;
        else
        DATA_RDWR <= '0';
        SEL2 <= "00";
        SEL1 <= '1';
        DATA_EN <= '1';
        PC_Inc <= '1';
        CPU_next <= S_START;
        end if ;
      
      when S_SAVE_TMP =>
        DATA_EN <= '1';
        TMP_Set <= '1';        
        DATA_RDWR <= '0';
        PC_Inc <= '1';
        CPU_next <=S_START;

      when S_SAVE =>
        DATA_RDWR <= '0';
        DATA_EN <= '1';
        SEL2 <= "01";
        SEL1 <= '1';
        PC_Inc <= '1';
        CPU_next <=S_START;

      when S_LOOP1 =>
        PC_Inc <= '1';      
        DATA_EN <= '1';      
        DATA_RDWR <= '1';
        SEL1 <= '1';  
        CPU_next <= S_LOOP2;
        

      when S_LOOP2 =>
        if (DATA_RDATA = "00000000") then
          CNT_Set <= '1';
          CPU_next <= S_LOOP3;        
        else
          CPU_next <= S_START;
        end if;  
    

      when S_LOOP3 =>
        if (CNT = "00000000") then
          PC_Dec <= '1';
          CPU_next <= S_START;
        else
          DATA_EN <= '1';
          SEL1 <= '0';
          DATA_RDWR <= '1';
          CPU_next <= S_LOOP4;
        end if;
      

      when S_LOOP4 =>
          if( instructions_pointer = I_LOOP_START) then
            CNT_Inc <= '1';
          elsif (instructions_pointer = I_LOOP_END) then
            CNT_Dec <= '1';
          end if;
          PC_Inc <= '1';
          CPU_next <= S_LOOP3;

      when S_LOOP_END1 =>
        DATA_EN <= '1';
        SEL1 <= '1';
        DATA_RDWR <= '1';
        CPU_next <= S_LOOP_END2;

      when S_LOOP_END2 =>
        if (DATA_RDATA = "00000000") then         
          PC_Inc <= '1';          
          CPU_next <= S_START;     
        else
          CNT_Set <= '1';
          PC_Dec <= '1';        
          CPU_next <= S_LOOP_END3;            
        end if;


      when S_LOOP_END3 =>
        if (CNT = "00000000") then
          PC_Inc <= '1';          
          CPU_next <= S_START;
        else 
          DATA_EN <= '1';
          SEL1 <= '0';
          DATA_RDWR <= '1';
          CPU_next <= S_LOOP_END4;
        end if;

      when S_LOOP_END4 => 
        if ( instructions_pointer = I_LOOP_END ) then
          CNT_Inc <= '1';
        elsif ( instructions_pointer = I_LOOP_START ) then
          CNT_Dec <= '1';
        end if;  
        PC_Dec <= '1';    
        CPU_next <= S_LOOP_END3;

    end case;
end process;






end behavioral;
 
