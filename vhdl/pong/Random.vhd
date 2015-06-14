LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY random IS
	PORT ( clk,clr	  : IN STD_LOGIC;
		   POSITION1, POSITION2   : OUT INTEGER RANGE 0 TO 48);
END random;

ARCHITECTURE behave OF random IS
		SIGNAL ranvar_1 : STD_LOGIC_VECTOR(7 DOWNTO 0); 
		SIGNAL ranvar_2 : STD_LOGIC_VECTOR(7 DOWNTO 0);
BEGIN
PROCESS(clk,clr)

BEGIN	
	if clr = '0' then
		ranvar_1 <= "00100101";
	--	ranvar_2 <= "00100101";
		
	elsIF clk'event and clk = '1' THEN
			ranvar_1 <= ranvar_1(7 DOWNTO 0);-- &PN_1
   	END IF;
		IF conv_integer(ranvar_1)< 32 THEN
		POSITION1<=16;
		POSITION2<= 16; 
		elsif (conv_integer(ranvar_1) < 40) and (conv_integer(ranvar_1) >31) then
		POSITION1<=17;
		POSITION2<= 17;
		elsif (conv_integer(ranvar_1) < 48) and (conv_integer(ranvar_1) >39) then
		POSITION1<=18;
		POSITION2<= 18; 
		elsif (conv_integer(ranvar_1) < 56) and (conv_integer(ranvar_1) >47) then
		POSITION1<=19;
		POSITION2<= 19;
		ELSIF (conv_integer(ranvar_1) < 64) AND (conv_integer(ranvar_1) > 55) THEN
		POSITION1<=20;
		POSITION2 <=20;
		elsif (conv_integer(ranvar_1) < 72) and (conv_integer(ranvar_1) >63) then
		POSITION1<=21;
		POSITION2<= 21;
		elsif (conv_integer(ranvar_1) < 80) and (conv_integer(ranvar_1) >71) then
		POSITION1<=22;
		POSITION2<= 22;
		elsif (conv_integer(ranvar_1) < 88) and (conv_integer(ranvar_1) >79) then
		POSITION1<=23;
		POSITION2<= 23;
		ELSIF (conv_integer(ranvar_1) < 96) AND (conv_integer(ranvar_1) > 87) THEN
		POSITION1<=24;
		POSITION2 <=24;
		elsif (conv_integer(ranvar_1) < 104) and (conv_integer(ranvar_1) >95) then
		POSITION1<=25;
		POSITION2<= 25;
		elsif (conv_integer(ranvar_1) < 112) and (conv_integer(ranvar_1) >103) then
		POSITION1<=26;
		POSITION2<= 26;
		elsif (conv_integer(ranvar_1) < 120) and (conv_integer(ranvar_1) >111) then
		POSITION1<=27;
		POSITION2<= 27;
		ELSIF (conv_integer(ranvar_1) < 128) AND (conv_integer(ranvar_1) > 119) THEN
		POSITION1<=28;
		POSITION2 <=28;
		elsif (conv_integer(ranvar_1) < 136) and (conv_integer(ranvar_1) >127) then
		POSITION1<=29;
		POSITION2<= 29;
		elsif (conv_integer(ranvar_1) < 144) and (conv_integer(ranvar_1) >135) then
		POSITION1<=30;
		POSITION2<= 30;
		elsif (conv_integer(ranvar_1) < 152) and (conv_integer(ranvar_1) >143) then
		POSITION1<=31;
		POSITION2<= 31;
		ELSIF (conv_integer(ranvar_1) < 160) AND (conv_integer(ranvar_1) > 151) THEN
		POSITION1<=32;
		POSITION2 <=32;
		elsif (conv_integer(ranvar_1) < 168) and (conv_integer(ranvar_1) >159) then
		POSITION1<=33;
		POSITION2<= 33;
		elsif (conv_integer(ranvar_1) < 176) and (conv_integer(ranvar_1) >167) then
		POSITION1<=34;
		POSITION2<= 34;
		elsif (conv_integer(ranvar_1) < 184) and (conv_integer(ranvar_1) >175) then
		POSITION1<=35;
		POSITION2<= 35;
		ELSIF (conv_integer(ranvar_1) < 192) AND (conv_integer(ranvar_1) > 183) THEN
		POSITION1<=36;
		POSITION2 <=36;
		elsif (conv_integer(ranvar_1) < 200) and (conv_integer(ranvar_1) >191) then
		POSITION1<=37;
		POSITION2<= 37;
		elsif (conv_integer(ranvar_1) < 208) and (conv_integer(ranvar_1) >199) then
		POSITION1<=38;
		POSITION2<= 38;
		elsif (conv_integer(ranvar_1) < 216) and (conv_integer(ranvar_1) >207) then
		POSITION1<=39;
		POSITION2<= 39;
		ELSIF (conv_integer(ranvar_1) < 224) AND (conv_integer(ranvar_1) > 215) THEN
		POSITION1<=40;
		POSITION2 <=40;
		elsif (conv_integer(ranvar_1) < 232) and (conv_integer(ranvar_1) >223) then
		POSITION1<=41;
		POSITION2<= 41;
		elsif (conv_integer(ranvar_1) < 240) and (conv_integer(ranvar_1) >231) then
		POSITION1<=42;
		POSITION2<= 42;
		elsif (conv_integer(ranvar_1) < 248) and (conv_integer(ranvar_1) >239) then
		POSITION1<=43;
		POSITION2<= 43;
		ELSIF (conv_integer(ranvar_1) > 247)  THEN
		POSITION1<=44;
		POSITION2 <=44;
		END IF;

END PROCESS;
END behave;