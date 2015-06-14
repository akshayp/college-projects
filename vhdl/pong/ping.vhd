LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;            --for using std_logic
USE IEEE.STD_LOGIC_UNSIGNED.ALL;        --for addition of std_logic
USE IEEE.STD_LOGIC_ARITH.ALL;
        
   ENTITY PING IS
       PORT( CLK,CLR              : IN STD_LOGIC;
             SERVE1, SERVE2       : IN STD_LOGIC; --Players 
			 POSITION1, PADDLE1	  : IN INTEGER RANGE 0 TO 48;
             POSITION2, PADDLE2	  : IN INTEGER RANGE 0 TO 48;
             SCORE11,SCORE22      : INOUT INTEGER RANGE 0 TO 9; --display score in waveform which is later converted to BCD
             ROW, COLUMN		  : OUT INTEGER RANGE 0 TO 64;                                                    
             OUT1,OUT2	          : OUT STD_LOGIC_VECTOR(6 DOWNTO 0); --BCD display of the score
             HEXOUT    	          : OUT STD_LOGIC_VECTOR(127 DOWNTO 0)); --Winners(P1,P0)
        
   END PING;
        
   ARCHITECTURE ALG OF PING IS
   	TYPE STATE_TYPE IS (SERVE_DECIDE,STEP_DECIDE_STATE_1,STEP_DECIDE_STATE_2,POSITION_STATE_1,POSITION_STATE_2,
   						LOOP_STATE_1,LOOP_STATE_2,SCORE_INC_1,SCORE_INC_2,IDLE_STATE_1,IDLE_STATE_2);
   	SIGNAL STATE : STATE_TYPE;
   	
    	BEGIN
		
        PROCESS(CLK,CLR)        --process wakes up on CLK or CLR 
        VARIABLE SCORE1 : INTEGER RANGE 0 TO 9; --temporary score
        VARIABLE SCORE2 : INTEGER RANGE 0 TO 9;
        VARIABLE CURRENT_ROW,PREVIOUS_ROW,CURRENT_COLUMN : INTEGER RANGE 0 TO 64;
        VARIABLE STEP : INTEGER RANGE 0 TO 64;
        variable a, b : integer range 1 to 4;
        BEGIN
			
        	IF CLR = '0' THEN
            	SCORE11 <= 0;  --the scores reset on CLR
                SCORE22 <= 0;
				HEXOUT <= x"20202020202020202020202020202020";
				SCORE1 := 1;
				SCORE2 := 1;
				CURRENT_ROW := 32;
                PREVIOUS_ROW := 32;
                CURRENT_COLUMN := 32;
                STATE <= SERVE_DECIDE;
            ELSIF CLK'EVENT AND CLK = '1' THEN
            	CASE STATE IS                	              	             	
                	WHEN SERVE_DECIDE =>
                    	IF serve1 = '0' THEN  --when player 1 serves
                        	STATE <= POSITION_STATE_1;                                                                        
                        ELSIF SERVE2 = '0' THEN  --when player 2 serves
                            STATE <= POSITION_STATE_2;                                                                        
                        ELSE
                            STATE <= SERVE_DECIDE;  --rest condition
                        END IF ;
                        
                        
               --PART DEALING WITH THE PLAYER 1
               
                    WHEN POSITION_STATE_1 =>
                    	if b = 1 then
						   	CURRENT_ROW := PADDLE1;
                    		PREVIOUS_ROW := PADDLE1;
						elsif b = 2 then
							CURRENT_ROW := PADDLE1+1;
                    		PREVIOUS_ROW := PADDLE1+1;
						elsif b =3 then
							CURRENT_ROW := PADDLE1-2;
                    		PREVIOUS_ROW := PADDLE1-2;
						elsif b = 4 then
							CURRENT_ROW := PADDLE1-1;
                    		PREVIOUS_ROW := PADDLE1-1;
						else
							CURRENT_ROW := PADDLE1;
                    		PREVIOUS_ROW := PADDLE1;
						end if;
                    	CURRENT_COLUMN := 16;
                    	STATE <= STEP_DECIDE_STATE_1;
                    WHEN STEP_DECIDE_STATE_1 =>
                    	CURRENT_ROW := POSITION1;
                    	CURRENT_COLUMN := CURRENT_COLUMN + 1; 
                    	IF POSITION1 >= PREVIOUS_ROW THEN
                    		STEP := (POSITION1 - PREVIOUS_ROW)/4;
                    	ELSE
                    		STEP := (PREVIOUS_ROW - POSITION1)/4;
                    	END IF;
                    	STATE <= LOOP_STATE_1;
                    WHEN LOOP_STATE_1 =>
                    	--ROW_IF
                    	IF CURRENT_ROW > PREVIOUS_ROW THEN
                    		IF (CURRENT_ROW + STEP) > 48 THEN
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := 48;
                    		ELSIF CURRENT_ROW = 48 THEN
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := CURRENT_ROW - STEP;
                    		ELSE
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := CURRENT_ROW + STEP;
                    		END IF;
                    	ELSE
							IF CURRENT_ROW = 16 THEN
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := CURRENT_ROW + STEP;
                    		ELSIF (CURRENT_ROW - STEP) < 16 THEN
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := 16;
                       		ELSE
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := CURRENT_ROW - STEP;
                    		END IF;
                    	END IF;
                    	--COLUMN_IF AND NEXT STATE DECISION
                    	IF CURRENT_COLUMN = 48 THEN
							CURRENT_COLUMN := CURRENT_COLUMN + 1;
                    		IF CURRENT_ROW > PREVIOUS_ROW THEN
								IF PADDLE2 = (CURRENT_ROW ) THEN--step
									a:=1;
                    				STATE <= POSITION_STATE_2;
								ELSIF ((paddle2 + 1) = current_row) THEN
									a:=2;
									STATE <= POSITION_STATE_2;
								ELSIF ((paddle2 - 2) = current_row) THEN
									a:=3;
									STATE <= POSITION_STATE_2;
								ELSIF ((paddle2 - 1) = current_row) THEN
									a:=4;
									STATE <= POSITION_STATE_2;
                    			ELSE
                    				STATE <= SCORE_INC_1;
                    			END IF;
                    		ELSE
                    			IF PADDLE2 = (CURRENT_ROW-step) THEN--step
									a := 1;
                    				STATE <= POSITION_STATE_2;
								ELSIF ((paddle2 + 1) = current_row) THEN
									a:=2;
									STATE <= POSITION_STATE_2;
								ELSIF ((paddle2 - 2) = current_row) THEN
									a:=3;
									STATE <= POSITION_STATE_2;
								ELSIF ((paddle2 - 1) = current_row) THEN
									a:=4;
									STATE <= POSITION_STATE_2;
                    			ELSE
                    				STATE <= SCORE_INC_1;
                    			END IF;
                    		END IF;
                    	ELSE
                    		CURRENT_COLUMN := CURRENT_COLUMN + 1;
                    		STATE <= LOOP_STATE_1;
                    	END IF;
                    WHEN SCORE_INC_1 =>
						 SCORE1 := SCORE11 + 1;
                         SCORE11 <= SCORE1;              --Player P1 gets the point
                         IF (SCORE11 + SCORE22) > 3 AND (SCORE11 + SCORE22) < 9 THEN     --This portion decide the serving player
                         	STATE <= IDLE_STATE_2;
                         ELSIF (SCORE11 + SCORE22) > 13 AND (SCORE11 + SCORE22) < 19 THEN
                         	STATE <= IDLE_STATE_2;
                         ELSIF (SCORE11 + SCORE22) > 23 AND (SCORE11 + SCORE22) < 30 THEN
                        	 STATE <= IDLE_STATE_2;
                         ELSE
                         	STATE <= IDLE_STATE_1;
                         END IF;
                    WHEN IDLE_STATE_1 =>
						 CURRENT_ROW := CURRENT_ROW;
						 CURRENT_COLUMN := CURRENT_COLUMN;
                         IF SCORE11 = 9 THEN
							HEXOUT <= X"506C6179657220312057696E73212121";
							STATE <= IDLE_STATE_1;
						 ELSIF SCORE22 = 9 THEN
							HEXOUT <= X"506C6179657220322057696E73212121";
                         	STATE <= IDLE_STATE_1;
                         ELSE	              --Player serves again
                         	STATE <= POSITION_STATE_1;
                         
                         END IF;
                    		
                    
                    	--PART DEALING WITH THE PLAYER 2
               
                    WHEN POSITION_STATE_2 =>
						IF a = 1 THEN
						   	CURRENT_ROW := PADDLE2;
                    		PREVIOUS_ROW := PADDLE2;
						ELSIF a = 2 THEN
							CURRENT_ROW := PADDLE2+1;
                    		PREVIOUS_ROW := PADDLE2+1;
						ELSIF a =3 THEN
							CURRENT_ROW := PADDLE2-2;
                    		PREVIOUS_ROW := PADDLE2-2;
						ELSIF a = 4 THEN
							CURRENT_ROW := PADDLE2-1;
                    		PREVIOUS_ROW := PADDLE2-1;
						ELSE
							CURRENT_ROW := PADDLE2;
                    		PREVIOUS_ROW := PADDLE2;
						END IF;
                    	CURRENT_COLUMN := 48;
                    	STATE <= STEP_DECIDE_STATE_2;
                    WHEN STEP_DECIDE_STATE_2 =>
                    	CURRENT_ROW := POSITION2;
                    	CURRENT_COLUMN := CURRENT_COLUMN - 1; 
                    	IF POSITION2 >= PREVIOUS_ROW THEN
                    		STEP := (POSITION2 - PREVIOUS_ROW)/4;
                    	ELSE
                    		STEP := (PREVIOUS_ROW - POSITION2)/4;
                    	END IF;
                    	STATE <= LOOP_STATE_2;
                    WHEN LOOP_STATE_2 =>
                    	--ROW_IF
                    	IF CURRENT_ROW > PREVIOUS_ROW THEN
                    		IF (CURRENT_ROW + STEP) > 48 THEN
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := 48;
                    		ELSIF CURRENT_ROW = 48 THEN
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := CURRENT_ROW - STEP;
                    		ELSE
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := CURRENT_ROW + STEP;
                    		END IF;
                    	ELSE
                    		IF CURRENT_ROW = 16 THEN
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := CURRENT_ROW + STEP;
							ELSIF (CURRENT_ROW - STEP) < 16 THEN
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := 16;
                    		ELSE
                    			PREVIOUS_ROW := CURRENT_ROW;
                    			CURRENT_ROW := CURRENT_ROW - STEP;
                    		END IF;
                    	END IF;
                    	--COLUMN_IF AND NEXT STATE DECISION
                    	IF CURRENT_COLUMN = 16 THEN
							CURRENT_COLUMN := CURRENT_COLUMN - 1;
                    		IF CURRENT_ROW > PREVIOUS_ROW THEN
                    			IF PADDLE1 = (CURRENT_ROW) THEN--+step
									b:=1;
                    				STATE <= POSITION_STATE_1;
                    			ELSif ((paddle1 + 1) = current_row) then
									b:=2;
									STATE <= POSITION_STATE_1;
								ELSif ((paddle1 - 2) = current_row) then
									b:=3;
									STATE <= POSITION_STATE_1;
								ELSif ((paddle1 - 1) = current_row) then
									b:=4;
									STATE <= POSITION_STATE_1;
								else
                    				STATE <= SCORE_INC_2;
                    			END IF;
                    		ELSE
                    			IF PADDLE1 = (CURRENT_ROW) THEN--step
                    				STATE <= POSITION_STATE_1;
								ELSif ((paddle1 + 1) = current_row) then
									STATE <= POSITION_STATE_1;
								ELSif ((paddle1 - 2) = current_row) then
									STATE <= POSITION_STATE_1;
								ELSif ((paddle1 - 1) = current_row) then
									STATE <= POSITION_STATE_1;
                    			ELSE
                    				STATE <= SCORE_INC_2;
                    			END IF;
                    		END IF;
                    	ELSE
                    		CURRENT_COLUMN := CURRENT_COLUMN - 1;
                    		STATE <= LOOP_STATE_2;
                    	END IF;
                    WHEN SCORE_INC_2 =>
						  SCORE2 := SCORE22 + 1;   
                          SCORE22 <= SCORE2;      --P0 gets the point
                          IF (SCORE11 + SCORE22) > 3 AND (SCORE11 + SCORE22) < 9 THEN      --CHANGE OF SERVICE
                             STATE <= IDLE_STATE_1;
                          ELSIF (SCORE11 + SCORE22) > 13 AND (SCORE11 + SCORE22) < 18 THEN
                             STATE <= IDLE_STATE_1;
                          ELSE
                             STATE <= IDLE_STATE_2;
                          END IF;
                         
                         
                    WHEN IDLE_STATE_2 =>
						 CURRENT_ROW := CURRENT_ROW;
						 CURRENT_COLUMN := CURRENT_COLUMN;
                         IF SCORE11 = 9 THEN
							HEXOUT <= X"506C6179657220312057696E73212121";
							STATE <= IDLE_STATE_2;
						 ELSIF SCORE22 = 9 THEN
							HEXOUT <= X"506C6179657220322057696E73212121";
                         	STATE <= IDLE_STATE_2;
                         ELSE	               --Player serves again
                         	STATE <= POSITION_STATE_2;
	                    
                         END IF;
                    		
					END CASE;
				END IF;
				ROW <= CURRENT_ROW;
				COLUMN <= CURRENT_COLUMN;
			END PROCESS;
			
			
		 PROCESS(SCORE11,SCORE22)                --to convert to BCD
                                BEGIN
                                        CASE SCORE11 IS
                                                WHEN 0 => OUT1 <= "0000001";  
                                                WHEN 1 => OUT1 <= "1001111";
                                                WHEN 2 => OUT1 <= "0010010";
                                                WHEN 3 => OUT1 <= "0000110";
                                                WHEN 4 => OUT1 <= "1001100";
                                                WHEN 5 => OUT1 <= "0100100";
                                                WHEN 6 => OUT1 <= "0100000";
                                                WHEN 7 => OUT1 <= "0001111";
                                                WHEN 8 => OUT1 <= "0000000";
                                                WHEN 9 => OUT1 <= "0001100";
                                                
                                        END CASE;
        
                                        CASE SCORE22 IS                         --score for Player P0
                                                WHEN 0 => OUT2 <= "0000001";
                                                WHEN 1 => OUT2 <= "1001111";
                                                WHEN 2 => OUT2 <= "0010010";
                                                WHEN 3 => OUT2 <= "0000110";
                                                WHEN 4 => OUT2 <= "1001100";
                                                WHEN 5 => OUT2 <= "0100100";
                                                WHEN 6 => OUT2 <= "0100000";
                                                WHEN 7 => OUT2 <= "0001111";
                                                WHEN 8 => OUT2 <= "0000000";
                                                WHEN 9 => OUT2 <= "0001100";
                                                
                                        END CASE;
        
                                END PROCESS;
   END ALG;