LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;

ENTITY display IS
        PORT(Clock	                    : IN STD_LOGIC;
        	  ROW, COLUMN				: IN INTEGER RANGE 0 TO 64;
        	  PADDLE1,PADDLE2			: IN INTEGER RANGE 0 TO 48;
        	  OUT1,OUT2           		: IN STD_LOGIC_VECTOR(6 DOWNTO 0);
                Horiz_sync, Vert_sync   : OUT STD_LOGIC;
                RED, GREEN, BLUE        : OUT STD_LOGIC);
        END display;
        
ARCHITECTURE NEW1 OF display IS

-- Video Display Signals   
signal Horiz_Count,Vert_Count: std_logic_vector(9 Downto 0);
signal R, G, B : std_logic;

-- Signals for Video Memory for Pixel Data
signal col_address, row_address: std_logic_vector(7 Downto 0);
signal col_count, row_count: std_logic_vector(2 Downto 0);
constant H_max : std_logic_vector(9 Downto 0) := CONV_STD_LOGIC_VECTOR(799,10); 
-- 799 is max horiz count
constant V_max : std_logic_vector(9 Downto 0) := CONV_STD_LOGIC_VECTOR(524,10); 
-- 524 is max vert count

begin           


Red <=   R ;
Green <= G;
Blue <=  B;

                                                                 

-----------------------------------------------------

--Generate Horizontal and Vertical Timing Signals for Video Signal
VIDEO_DISPLAY: Process
Begin

Wait until(Clock'Event and Clock='1');

-- Horiz_Count counts pixels (640 + extra time for sync signals)
If (Horiz_Count >= H_max) then
   Horiz_Count <= To_Stdlogicvector(B"0000000000");
Else
   Horiz_Count <= Horiz_Count + To_Stdlogicvector(B"0000000001");
End if;

--Generate Horizontal Sync Signal
If (Horiz_Count <= CONV_STD_LOGIC_VECTOR(755,10)) and (Horiz_Count >= CONV_STD_LOGIC_VECTOR(659,10)) Then
   Horiz_Sync <= '0';
ELSE
   Horiz_Sync <= '1';
End if;

--Vert_Count counts rows of pixels (480 + extra time for sync signals)
If (Vert_Count >= V_max) and (Horiz_Count >= CONV_STD_LOGIC_VECTOR(699,10)) then
   Vert_Count <= To_Stdlogicvector(B"0000000000");
Else If (Horiz_Count = CONV_STD_LOGIC_VECTOR(699,10)) Then
   Vert_Count <= Vert_Count + To_Stdlogicvector(B"0000000001");
End if;


-- Generate Vertical Sync Signal
If (Vert_Count <= CONV_STD_LOGIC_VECTOR(494,10)) and (Vert_Count >= CONV_STD_LOGIC_VECTOR(493,10)) Then
   Vert_Sync <= '0';
ELSE
   Vert_Sync <= '1';
End if;

-- Generate row and col address for 7 by 5 superpixel to map into 64 by 64 video memory

if (Horiz_Count <= CONV_STD_LOGIC_VECTOR(639,10)) Then
   If col_count < CONV_STD_LOGIC_VECTOR(7,3) Then
   		col_count <= col_count + '1';
   Else
   		col_count <= "000";
   		col_address <= col_address + '1';
   End if;
ELSE
   col_count <= "000";
   col_address <= "00000000";
End if;

IF(HORIZ_COUNT = CONV_STD_LOGIC_VECTOR(641,10)) Then
    row_count <= row_count + '1';
If (row_count = CONV_STD_LOGIC_VECTOR(5,3)) THEN
   row_count <= "000";
   row_address <= row_address + '1';
End if;
End if;

If (Vert_Count >= CONV_STD_LOGIC_VECTOR(479,10)) Then
   row_count <= "000";
   row_address <= "00000000";
End if;
end if;
end process VIDEO_DISPLAY;

RGB:PROCESS(PADDLE1, PADDLE2, ROW, COLUMN, OUT1, OUT2)
	BEGIN
		--BOUNDARY
		IF row_address = CONV_STD_LOGIC_VECTOR(15,8) AND col_address >= CONV_STD_LOGIC_VECTOR(15,8) AND col_address <=  CONV_STD_LOGIC_VECTOR(89,8) THEN
			R <= '1';
			G <= '0';
			B <= '0';
		ELSIF row_address = CONV_STD_LOGIC_VECTOR(49,8) AND col_address >= CONV_STD_LOGIC_VECTOR(15,8) AND col_address <=  CONV_STD_LOGIC_VECTOR(89,8) THEN
			R <= '1';
			G <= '0';
			B <= '0';
		--ELSIF col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND row_address >= CONV_STD_LOGIC_VECTOR(15,8) AND row_address <= CONV_STD_LOGIC_VECTOR(89,8) THEN
		--	R <= '1';
		--	G <= '0';
		--	B <= '0';
		--ELSIF col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND row_address >=  CONV_STD_LOGIC_VECTOR(15,8) AND row_address <=  CONV_STD_LOGIC_VECTOR(89,8) THEN
		--	R <= '1';
		--	G <= '0';
		--	B <= '0';
		END IF;
		
	--PADDLE1
		
			FOR i IN 0 TO 32 LOOP
				IF col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(15+i,8) OR row_address = CONV_STD_LOGIC_VECTOR(16+i,8) OR row_address = CONV_STD_LOGIC_VECTOR(17+i,8) OR row_address = CONV_STD_LOGIC_VECTOR(18+i,8)) THEN
					R <= '1';
					G <= '0';
					B <= '1';
				END IF;	
	
			END loop;
	
	--PADDLE2
			FOR i IN 0 TO 32 LOOP
				IF col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(15+i,8) OR row_address = CONV_STD_LOGIC_VECTOR(16+i,8) OR row_address = CONV_STD_LOGIC_VECTOR(17+i,8) OR row_address = CONV_STD_LOGIC_VECTOR(18+i,8)) THEN
					R <= '1';
					G <= '0';
					B <= '1';
				END IF;
			END LOOP;	


--BALL
	IF row_address = CONV_STD_LOGIC_VECTOR(ROW,8) AND col_address = CONV_STD_LOGIC_VECTOR(COLUMN,8) THEN
		R <= '1';
		G <= '1';
		B <= '0';
	END IF;		
	
--SCORES
--SCORE OF PLAYER 1
	IF OUT1(6) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(29,8) AND (col_address = CONV_STD_LOGIC_VECTOR(8,8) OR col_address = CONV_STD_LOGIC_VECTOR(9,8) OR col_address = CONV_STD_LOGIC_VECTOR(10,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT1(5) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(10,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT1(4) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(10,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT1(3) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(33,8) AND (col_address = CONV_STD_LOGIC_VECTOR(8,8) OR col_address = CONV_STD_LOGIC_VECTOR(9,8) OR col_address = CONV_STD_LOGIC_VECTOR(10,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT1(2) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(8,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT1(1) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(8,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT1(0) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(31,8) AND (col_address = CONV_STD_LOGIC_VECTOR(8,8) OR col_address = CONV_STD_LOGIC_VECTOR(9,8) OR col_address = CONV_STD_LOGIC_VECTOR(10,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	END IF;
	
--SCORE OF PLAYER 2
	IF OUT2(6) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(29,8) AND (col_address = CONV_STD_LOGIC_VECTOR(54,8) OR col_address = CONV_STD_LOGIC_VECTOR(55,8) OR col_address = CONV_STD_LOGIC_VECTOR(56,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT2(5) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(56,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT2(4) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(56,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT2(3) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(33,8) AND (col_address = CONV_STD_LOGIC_VECTOR(54,8) OR col_address = CONV_STD_LOGIC_VECTOR(55,8) OR col_address = CONV_STD_LOGIC_VECTOR(56,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT2(2) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(54,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT2(1) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(54,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	ELSIF OUT2(0) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(31,8) AND (col_address = CONV_STD_LOGIC_VECTOR(54,8) OR col_address = CONV_STD_LOGIC_VECTOR(55,8) OR col_address = CONV_STD_LOGIC_VECTOR(56,8)) THEN
		R <= '1';
		G <='1';
		B <= '1';
	END IF;

--REST OF THE SCREEN IS BLACKENED SO THAT THERE IS LESS POWER CONSUMPTION FROM THE SCREEN
IF (NOT(row_address = CONV_STD_LOGIC_VECTOR(15,8) AND col_address >= CONV_STD_LOGIC_VECTOR(15,8) AND col_address <=  CONV_STD_LOGIC_VECTOR(49,8))AND
	NOT(row_address = CONV_STD_LOGIC_VECTOR(49,8) AND col_address >= CONV_STD_LOGIC_VECTOR(15,8) AND col_address <=  CONV_STD_LOGIC_VECTOR(49,8))AND
	--NOT(col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND row_address >= CONV_STD_LOGIC_VECTOR(15,8) AND row_address <= CONV_STD_LOGIC_VECTOR(49,8))AND
	--NOT(col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND row_address >=  CONV_STD_LOGIC_VECTOR(15,8) AND row_address <=  CONV_STD_LOGIC_VECTOR(49,8))AND
	NOT(PADDLE1=17 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(15,8) OR row_address = CONV_STD_LOGIC_VECTOR(16,8) OR row_address = CONV_STD_LOGIC_VECTOR(17,8) OR row_address = CONV_STD_LOGIC_VECTOR(18,8)))AND
	NOT(PADDLE1=18 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(16,8) OR row_address = CONV_STD_LOGIC_VECTOR(17,8) OR row_address = CONV_STD_LOGIC_VECTOR(18,8) OR row_address = CONV_STD_LOGIC_VECTOR(19,8)))AND
	NOT(PADDLE1=19 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(17,8) OR row_address = CONV_STD_LOGIC_VECTOR(18,8) OR row_address = CONV_STD_LOGIC_VECTOR(19,8) OR row_address = CONV_STD_LOGIC_VECTOR(20,8)))AND
	NOT(PADDLE1=20 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(18,8) OR row_address = CONV_STD_LOGIC_VECTOR(19,8) OR row_address = CONV_STD_LOGIC_VECTOR(20,8) OR row_address = CONV_STD_LOGIC_VECTOR(21,8)))AND
	NOT(PADDLE1=21 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(19,8) OR row_address = CONV_STD_LOGIC_VECTOR(20,8) OR row_address = CONV_STD_LOGIC_VECTOR(21,8) OR row_address = CONV_STD_LOGIC_VECTOR(22,8)))AND
	NOT(PADDLE1=22 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(20,8) OR row_address = CONV_STD_LOGIC_VECTOR(21,8) OR row_address = CONV_STD_LOGIC_VECTOR(22,8) OR row_address = CONV_STD_LOGIC_VECTOR(23,8)))AND
	NOT(PADDLE1=23 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(21,8) OR row_address = CONV_STD_LOGIC_VECTOR(22,8) OR row_address = CONV_STD_LOGIC_VECTOR(23,8) OR row_address = CONV_STD_LOGIC_VECTOR(24,8)))AND
	NOT(PADDLE1=24 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(22,8) OR row_address = CONV_STD_LOGIC_VECTOR(23,8) OR row_address = CONV_STD_LOGIC_VECTOR(24,8) OR row_address = CONV_STD_LOGIC_VECTOR(25,8)))AND
	NOT(PADDLE1=25 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(23,8) OR row_address = CONV_STD_LOGIC_VECTOR(24,8) OR row_address = CONV_STD_LOGIC_VECTOR(25,8) OR row_address = CONV_STD_LOGIC_VECTOR(26,8)))AND
	NOT(PADDLE1=26 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(24,8) OR row_address = CONV_STD_LOGIC_VECTOR(25,8) OR row_address = CONV_STD_LOGIC_VECTOR(26,8) OR row_address = CONV_STD_LOGIC_VECTOR(27,8)))AND
	NOT(PADDLE1=27 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(25,8) OR row_address = CONV_STD_LOGIC_VECTOR(26,8) OR row_address = CONV_STD_LOGIC_VECTOR(27,8) OR row_address = CONV_STD_LOGIC_VECTOR(28,8)))AND
	NOT(PADDLE1=28 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(26,8) OR row_address = CONV_STD_LOGIC_VECTOR(27,8) OR row_address = CONV_STD_LOGIC_VECTOR(28,8) OR row_address = CONV_STD_LOGIC_VECTOR(29,8)))AND
	NOT(PADDLE1=29 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(27,8) OR row_address = CONV_STD_LOGIC_VECTOR(28,8) OR row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8)))AND
	NOT(PADDLE1=30 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(28,8) OR row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)))AND
	NOT(PADDLE1=31 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8)))AND
	NOT(PADDLE1=32 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)))AND
	NOT(PADDLE1=33 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8) OR row_address = CONV_STD_LOGIC_VECTOR(34,8)))AND
	NOT(PADDLE1=34 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8) OR row_address = CONV_STD_LOGIC_VECTOR(34,8) OR row_address = CONV_STD_LOGIC_VECTOR(35,8)))AND
	NOT(PADDLE1=35 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(33,8) OR row_address = CONV_STD_LOGIC_VECTOR(34,8) OR row_address = CONV_STD_LOGIC_VECTOR(35,8) OR row_address = CONV_STD_LOGIC_VECTOR(36,8)))AND
	NOT(PADDLE1=36 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(34,8) OR row_address = CONV_STD_LOGIC_VECTOR(35,8) OR row_address = CONV_STD_LOGIC_VECTOR(36,8) OR row_address = CONV_STD_LOGIC_VECTOR(37,8)))AND
	NOT(PADDLE1=37 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(35,8) OR row_address = CONV_STD_LOGIC_VECTOR(36,8) OR row_address = CONV_STD_LOGIC_VECTOR(37,8) OR row_address = CONV_STD_LOGIC_VECTOR(38,8)))AND
	NOT(PADDLE1=38 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(36,8) OR row_address = CONV_STD_LOGIC_VECTOR(37,8) OR row_address = CONV_STD_LOGIC_VECTOR(38,8) OR row_address = CONV_STD_LOGIC_VECTOR(39,8)))AND
	NOT(PADDLE1=39 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(37,8) OR row_address = CONV_STD_LOGIC_VECTOR(38,8) OR row_address = CONV_STD_LOGIC_VECTOR(39,8) OR row_address = CONV_STD_LOGIC_VECTOR(40,8)))AND
	NOT(PADDLE1=40 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(38,8) OR row_address = CONV_STD_LOGIC_VECTOR(39,8) OR row_address = CONV_STD_LOGIC_VECTOR(40,8) OR row_address = CONV_STD_LOGIC_VECTOR(41,8)))AND
	NOT(PADDLE1=41 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(39,8) OR row_address = CONV_STD_LOGIC_VECTOR(40,8) OR row_address = CONV_STD_LOGIC_VECTOR(41,8) OR row_address = CONV_STD_LOGIC_VECTOR(42,8)))AND
	NOT(PADDLE1=42 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(40,8) OR row_address = CONV_STD_LOGIC_VECTOR(41,8) OR row_address = CONV_STD_LOGIC_VECTOR(42,8) OR row_address = CONV_STD_LOGIC_VECTOR(43,8)))AND
	NOT(PADDLE1=43 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(41,8) OR row_address = CONV_STD_LOGIC_VECTOR(42,8) OR row_address = CONV_STD_LOGIC_VECTOR(43,8) OR row_address = CONV_STD_LOGIC_VECTOR(44,8)))AND
	NOT(PADDLE1=44 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(42,8) OR row_address = CONV_STD_LOGIC_VECTOR(43,8) OR row_address = CONV_STD_LOGIC_VECTOR(44,8) OR row_address = CONV_STD_LOGIC_VECTOR(45,8)))AND
	NOT(PADDLE1=45 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(43,8) OR row_address = CONV_STD_LOGIC_VECTOR(44,8) OR row_address = CONV_STD_LOGIC_VECTOR(45,8) OR row_address = CONV_STD_LOGIC_VECTOR(46,8)))AND
	NOT(PADDLE1=46 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(44,8) OR row_address = CONV_STD_LOGIC_VECTOR(45,8) OR row_address = CONV_STD_LOGIC_VECTOR(46,8) OR row_address = CONV_STD_LOGIC_VECTOR(47,8)))AND
	NOT(PADDLE1=47 AND col_address =  CONV_STD_LOGIC_VECTOR(15,8) AND (row_address = CONV_STD_LOGIC_VECTOR(45,8) OR row_address = CONV_STD_LOGIC_VECTOR(46,8) OR row_address = CONV_STD_LOGIC_VECTOR(47,8) OR row_address = CONV_STD_LOGIC_VECTOR(48,8)))AND
	NOT(PADDLE2=17 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(15,8) OR row_address = CONV_STD_LOGIC_VECTOR(16,8) OR row_address = CONV_STD_LOGIC_VECTOR(17,8) OR row_address = CONV_STD_LOGIC_VECTOR(18,8)))AND
	NOT(PADDLE2=18 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(16,8) OR row_address = CONV_STD_LOGIC_VECTOR(17,8) OR row_address = CONV_STD_LOGIC_VECTOR(18,8) OR row_address = CONV_STD_LOGIC_VECTOR(19,8)))AND
	NOT(PADDLE2=19 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(17,8) OR row_address = CONV_STD_LOGIC_VECTOR(18,8) OR row_address = CONV_STD_LOGIC_VECTOR(19,8) OR row_address = CONV_STD_LOGIC_VECTOR(20,8)))AND
	NOT(PADDLE2=20 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(18,8) OR row_address = CONV_STD_LOGIC_VECTOR(19,8) OR row_address = CONV_STD_LOGIC_VECTOR(20,8) OR row_address = CONV_STD_LOGIC_VECTOR(21,8)))AND
	NOT(PADDLE2=21 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(19,8) OR row_address = CONV_STD_LOGIC_VECTOR(20,8) OR row_address = CONV_STD_LOGIC_VECTOR(21,8) OR row_address = CONV_STD_LOGIC_VECTOR(22,8)))AND
	NOT(PADDLE2=22 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(20,8) OR row_address = CONV_STD_LOGIC_VECTOR(21,8) OR row_address = CONV_STD_LOGIC_VECTOR(22,8) OR row_address = CONV_STD_LOGIC_VECTOR(23,8)))AND
	NOT(PADDLE2=23 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(21,8) OR row_address = CONV_STD_LOGIC_VECTOR(22,8) OR row_address = CONV_STD_LOGIC_VECTOR(23,8) OR row_address = CONV_STD_LOGIC_VECTOR(24,8)))AND
	NOT(PADDLE2=24 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(22,8) OR row_address = CONV_STD_LOGIC_VECTOR(23,8) OR row_address = CONV_STD_LOGIC_VECTOR(24,8) OR row_address = CONV_STD_LOGIC_VECTOR(25,8)))AND
	NOT(PADDLE2=25 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(23,8) OR row_address = CONV_STD_LOGIC_VECTOR(24,8) OR row_address = CONV_STD_LOGIC_VECTOR(25,8) OR row_address = CONV_STD_LOGIC_VECTOR(26,8)))AND
	NOT(PADDLE2=26 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(24,8) OR row_address = CONV_STD_LOGIC_VECTOR(25,8) OR row_address = CONV_STD_LOGIC_VECTOR(26,8) OR row_address = CONV_STD_LOGIC_VECTOR(27,8)))AND
	NOT(PADDLE2=27 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(25,8) OR row_address = CONV_STD_LOGIC_VECTOR(26,8) OR row_address = CONV_STD_LOGIC_VECTOR(27,8) OR row_address = CONV_STD_LOGIC_VECTOR(28,8)))AND
	NOT(PADDLE2=28 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(26,8) OR row_address = CONV_STD_LOGIC_VECTOR(27,8) OR row_address = CONV_STD_LOGIC_VECTOR(28,8) OR row_address = CONV_STD_LOGIC_VECTOR(29,8)))AND
	NOT(PADDLE2=29 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(27,8) OR row_address = CONV_STD_LOGIC_VECTOR(28,8) OR row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8)))AND
	NOT(PADDLE2=30 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(28,8) OR row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)))AND
	NOT(PADDLE2=31 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8)))AND
	NOT(PADDLE2=32 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)))AND
	NOT(PADDLE2=33 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8) OR row_address = CONV_STD_LOGIC_VECTOR(34,8)))AND
	NOT(PADDLE2=34 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8) OR row_address = CONV_STD_LOGIC_VECTOR(34,8) OR row_address = CONV_STD_LOGIC_VECTOR(35,8)))AND
	NOT(PADDLE2=35 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(33,8) OR row_address = CONV_STD_LOGIC_VECTOR(34,8) OR row_address = CONV_STD_LOGIC_VECTOR(35,8) OR row_address = CONV_STD_LOGIC_VECTOR(36,8)))AND
	NOT(PADDLE2=36 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(34,8) OR row_address = CONV_STD_LOGIC_VECTOR(35,8) OR row_address = CONV_STD_LOGIC_VECTOR(36,8) OR row_address = CONV_STD_LOGIC_VECTOR(37,8)))AND
	NOT(PADDLE2=37 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(35,8) OR row_address = CONV_STD_LOGIC_VECTOR(36,8) OR row_address = CONV_STD_LOGIC_VECTOR(37,8) OR row_address = CONV_STD_LOGIC_VECTOR(38,8)))AND
	NOT(PADDLE2=38 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(36,8) OR row_address = CONV_STD_LOGIC_VECTOR(37,8) OR row_address = CONV_STD_LOGIC_VECTOR(38,8) OR row_address = CONV_STD_LOGIC_VECTOR(39,8)))AND
	NOT(PADDLE2=39 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(37,8) OR row_address = CONV_STD_LOGIC_VECTOR(38,8) OR row_address = CONV_STD_LOGIC_VECTOR(39,8) OR row_address = CONV_STD_LOGIC_VECTOR(40,8)))AND
	NOT(PADDLE2=40 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(38,8) OR row_address = CONV_STD_LOGIC_VECTOR(39,8) OR row_address = CONV_STD_LOGIC_VECTOR(40,8) OR row_address = CONV_STD_LOGIC_VECTOR(41,8)))AND
	NOT(PADDLE2=41 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(39,8) OR row_address = CONV_STD_LOGIC_VECTOR(40,8) OR row_address = CONV_STD_LOGIC_VECTOR(41,8) OR row_address = CONV_STD_LOGIC_VECTOR(42,8)))AND
	NOT(PADDLE2=42 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(40,8) OR row_address = CONV_STD_LOGIC_VECTOR(41,8) OR row_address = CONV_STD_LOGIC_VECTOR(42,8) OR row_address = CONV_STD_LOGIC_VECTOR(43,8)))AND
	NOT(PADDLE2=43 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(41,8) OR row_address = CONV_STD_LOGIC_VECTOR(42,8) OR row_address = CONV_STD_LOGIC_VECTOR(43,8) OR row_address = CONV_STD_LOGIC_VECTOR(44,8)))AND
	NOT(PADDLE2=44 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(42,8) OR row_address = CONV_STD_LOGIC_VECTOR(43,8) OR row_address = CONV_STD_LOGIC_VECTOR(44,8) OR row_address = CONV_STD_LOGIC_VECTOR(45,8)))AND
	NOT(PADDLE2=45 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(43,8) OR row_address = CONV_STD_LOGIC_VECTOR(44,8) OR row_address = CONV_STD_LOGIC_VECTOR(45,8) OR row_address = CONV_STD_LOGIC_VECTOR(46,8)))AND
	NOT(PADDLE2=46 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(44,8) OR row_address = CONV_STD_LOGIC_VECTOR(45,8) OR row_address = CONV_STD_LOGIC_VECTOR(46,8) OR row_address = CONV_STD_LOGIC_VECTOR(47,8)))AND
	NOT(PADDLE2=47 AND col_address =  CONV_STD_LOGIC_VECTOR(49,8) AND (row_address = CONV_STD_LOGIC_VECTOR(45,8) OR row_address = CONV_STD_LOGIC_VECTOR(46,8) OR row_address = CONV_STD_LOGIC_VECTOR(47,8) OR row_address = CONV_STD_LOGIC_VECTOR(48,8)))AND
	NOT(row_address = CONV_STD_LOGIC_VECTOR(ROW,8) AND col_address = CONV_STD_LOGIC_VECTOR(COLUMN,8)) AND
	NOT(OUT1(6) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(29,8) AND (col_address = CONV_STD_LOGIC_VECTOR(8,8) OR col_address = CONV_STD_LOGIC_VECTOR(9,8) OR col_address = CONV_STD_LOGIC_VECTOR(10,8)))AND
	NOT(OUT1(5) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(10,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)))AND
	NOT(OUT1(4) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(10,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)))AND
	NOT(OUT1(3) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(33,8) AND (col_address = CONV_STD_LOGIC_VECTOR(8,8) OR col_address = CONV_STD_LOGIC_VECTOR(9,8) OR col_address = CONV_STD_LOGIC_VECTOR(10,8)))AND
	NOT(OUT1(2) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(8,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)))AND
	NOT(OUT1(1) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(8,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)))AND
	NOT(OUT1(0) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(31,8) AND (col_address = CONV_STD_LOGIC_VECTOR(8,8) OR col_address = CONV_STD_LOGIC_VECTOR(9,8) OR col_address = CONV_STD_LOGIC_VECTOR(10,8)))AND
	NOT(OUT2(6) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(29,8) AND (col_address = CONV_STD_LOGIC_VECTOR(54,8) OR col_address = CONV_STD_LOGIC_VECTOR(55,8) OR col_address = CONV_STD_LOGIC_VECTOR(56,8)))AND
	NOT(OUT2(5) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(56,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)))AND
	NOT(OUT2(4) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(56,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)))AND
	NOT(OUT2(3) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(33,8) AND (col_address = CONV_STD_LOGIC_VECTOR(54,8) OR col_address = CONV_STD_LOGIC_VECTOR(55,8) OR col_address = CONV_STD_LOGIC_VECTOR(56,8)))AND
	NOT(OUT2(2) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(54,8) AND (row_address = CONV_STD_LOGIC_VECTOR(31,8) OR row_address = CONV_STD_LOGIC_VECTOR(32,8) OR row_address = CONV_STD_LOGIC_VECTOR(33,8)))AND
	NOT(OUT2(1) = '0' AND col_address = CONV_STD_LOGIC_VECTOR(54,8) AND (row_address = CONV_STD_LOGIC_VECTOR(29,8) OR row_address = CONV_STD_LOGIC_VECTOR(30,8) OR row_address = CONV_STD_LOGIC_VECTOR(31,8)))AND
	NOT(OUT2(0) = '0' AND row_address = CONV_STD_LOGIC_VECTOR(31,8) AND (col_address = CONV_STD_LOGIC_VECTOR(54,8) OR col_address = CONV_STD_LOGIC_VECTOR(55,8) OR col_address = CONV_STD_LOGIC_VECTOR(56,8)))) THEN
		R <= '0';
		G <= '0';
		B <= '0';
	END IF;

	END PROCESS RGB;
END NEW1;