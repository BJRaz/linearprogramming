100 ' ╔════════════════════════════════════════════════════════════════════════════════╗
110 ' ║              LINEAR PROGRAMMING SOLVER - AMIGA BASIC (POC)                      ║
120 ' ║                                                                                ║
130 ' ║  This program demonstrates that the C++ Linear Programming Solver can be       ║
140 ' ║  ported to Amiga Basic, showcasing platform independence of algorithmic       ║
150 ' ║  design. It implements a simplex method solver for linear programming         ║
160 ' ║  problems in standard format.                                                  ║
170 ' ║                                                                                ║
180 ' ║  Features:                                                                     ║
190 ' ║    - Lexical analysis (tokenization)                                          ║
200 ' ║    - Recursive descent parsing                                                ║
210 ' ║    - Simplex algorithm solver                                                 ║
220 ' ║    - Matrix operations and pivoting                                           ║
230 ' ║    - Comment support (// and /* */)                                           ║
240 ' ║    - Constraint handling (<, >, =)                                            ║
250 ' ║                                                                                ║
260 ' ╚════════════════════════════════════════════════════════════════════════════════╝

270 GOTO 1000  ' Skip to main program

280 ' ════════════════════════════════════════════════════════════════════════════════
290 ' DATA STRUCTURES AND GLOBAL VARIABLES
300 ' ════════════════════════════════════════════════════════════════════════════════

310 ' Token types
320 CONST TOKTYPE_NUMBER = 1
330 CONST TOKTYPE_IDENT = 2
340 CONST TOKTYPE_PLUS = 3
350 CONST TOKTYPE_MINUS = 4
360 CONST TOKTYPE_LESS = 5
370 CONST TOKTYPE_GREATER = 6
380 CONST TOKTYPE_EQUAL = 7
390 CONST TOKTYPE_SEMICOLON = 8
400 CONST TOKTYPE_EOF = 9

410 ' Constraint operator types
420 CONST CONTYPE_LESS = 1
430 CONST CONTYPE_GREATER = 2
440 CONST CONTYPE_EQUAL = 3

450 ' Limits
460 CONST MAXVAR = 10
470 CONST MAXCON = 10
480 CONST MAXTOK = 200
490 CONST EPSILON = 0.0001

500 ' Token structure (arrays)
510 DIM tokenType(MAXTOK)
520 DIM tokenValue$(MAXTOK)
530 SHARED tokenCount, tokenPos

540 ' Constraint structure (parallel arrays)
550 DIM constraintCoeff(MAXCON, MAXVAR)  ' Coefficients
560 DIM constraintOp(MAXCON)              ' Operator type
570 DIM constraintRhs(MAXCON)             ' Right-hand side
580 SHARED numConstraints

590 ' Objective function
600 DIM objectiveCoeff(MAXVAR)
610 SHARED numVariables

620 ' Variable tracking
630 DIM varName$(MAXVAR)
640 SHARED varCount

650 ' Simplex tableau
660 DIM tableau(MAXCON+1, MAXVAR+MAXCON+2)
670 DIM basisVar(MAXCON)
670 SHARED tableauRows, tableauCols

680 ' Solution
690 DIM solution(MAXVAR)
700 SHARED isOptimal, optimalValue

710 SHARED problemFile$, fileContent$, curPos

720 ' ════════════════════════════════════════════════════════════════════════════════
730 ' MAIN PROGRAM LOOP
740 ' ════════════════════════════════════════════════════════════════════════════════

1000 PRINT CHR$(147)  ' Clear screen
1010 PRINT "╔════════════════════════════════════════════════════════════════╗"
1020 PRINT "║  Linear Programming Solver - Amiga Basic (POC)                 ║"
1030 PRINT "╚════════════════════════════════════════════════════════════════╝"
1040 PRINT ""

1050 INPUT "Enter problem file path (or 'demo' for built-in example): ", problemFile$

1060 IF problemFile$ = "demo" THEN
1070   GOSUB 1200
1080 ELSE
1090   GOSUB 1100
1100 END IF

1110 GOSUB 1300  ' Tokenize
1120 GOSUB 1500  ' Parse
1130 GOSUB 2000  ' Solve
1140 GOSUB 2400  ' Print results

1150 PRINT ""
1160 INPUT "Solve another problem? (Y/N): ", ans$
1170 IF UCASE$(ans$) = "Y" THEN GOTO 1000
1180 PRINT "Goodbye!"
1190 END

1200 ' Demo mode with built-in problem
1210 fileContent$ = "+ 1 x + 1 y < 400 ;" + CHR$(10) + "+ 2 x + 1 y < 700 ;" + CHR$(10) + "+ 100 x + 100 y = 0 ;"
1220 RETURN

1300 ' Read file
1310 ON ERROR GOTO 1350
1320 OPEN "R", 1, problemFile$, 256
1330 fileContent$ = ""
1340 DO WHILE NOT EOF(1)
1350   INPUT #1, line$
1360   fileContent$ = fileContent$ + line$ + CHR$(10)
1370 LOOP
1380 CLOSE 1
1390 RETURN

1350 ' Error handling
1360 PRINT "Error: Could not read file '"; problemFile$; "'"
1370 RETURN

1400 ' ════════════════════════════════════════════════════════════════════════════════
1410 ' LEXICAL ANALYZER (TOKENIZER)
1420 ' ════════════════════════════════════════════════════════════════════════════════

1500 REM Tokenize the input
1510 curPos = 1
1520 tokenCount = 0
1530 
1540 DO WHILE curPos <= LEN(fileContent$)
1550   ch$ = MID$(fileContent$, curPos, 1)
1560   
1570   ' Skip whitespace
1580   IF ch$ = " " OR ch$ = CHR$(9) OR ch$ = CHR$(10) OR ch$ = CHR$(13) THEN
1590     curPos = curPos + 1
1600     GOTO 1540
1610   END IF
1620   
1630   ' Handle line comments (//)
1640   IF MID$(fileContent$, curPos, 2) = "//" THEN
1650     DO WHILE curPos <= LEN(fileContent$) AND MID$(fileContent$, curPos, 1) <> CHR$(10)
1660       curPos = curPos + 1
1670     LOOP
1680     GOTO 1540
1690   END IF
1700   
1710   ' Handle block comments (/* */)
1720   IF MID$(fileContent$, curPos, 2) = "/*" THEN
1730     DO WHILE curPos < LEN(fileContent$) - 1
1740       IF MID$(fileContent$, curPos, 2) = "*/" THEN
1750         curPos = curPos + 2
1760         GOTO 1540
1770       END IF
1780       curPos = curPos + 1
1790     LOOP
1800     GOTO 1540
1810   END IF
1820   
1830   ' Number token
1840   IF ch$ >= "0" AND ch$ <= "9" THEN
1850     startPos = curPos
1860     DO WHILE curPos <= LEN(fileContent$)
1870       ch$ = MID$(fileContent$, curPos, 1)
1880       IF (ch$ >= "0" AND ch$ <= "9") OR ch$ = "." THEN
1890         curPos = curPos + 1
1900       ELSE
1910         EXIT DO
1920       END IF
1930     LOOP
1940     tokenType(tokenCount) = TOKTYPE_NUMBER
1950     tokenValue$(tokenCount) = MID$(fileContent$, startPos, curPos - startPos)
1960     tokenCount = tokenCount + 1
1970     GOTO 1540
1980   END IF
1990   
2000   ' Identifier token
2010   IF (ch$ >= "A" AND ch$ <= "Z") OR (ch$ >= "a" AND ch$ <= "z") OR ch$ = "_" THEN
2020     startPos = curPos
2030     DO WHILE curPos <= LEN(fileContent$)
2040       ch$ = MID$(fileContent$, curPos, 1)
2050       IF (ch$ >= "A" AND ch$ <= "Z") OR (ch$ >= "a" AND ch$ <= "z") OR _
2060          (ch$ >= "0" AND ch$ <= "9") OR ch$ = "_" THEN
2070         curPos = curPos + 1
2080       ELSE
2090         EXIT DO
2100       END IF
2110     LOOP
2120     tokenType(tokenCount) = TOKTYPE_IDENT
2130     tokenValue$(tokenCount) = MID$(fileContent$, startPos, curPos - startPos)
2140     tokenCount = tokenCount + 1
2150     GOTO 1540
2160   END IF
2170   
2180   ' Single character tokens
2190   SELECT CASE ch$
2200     CASE "+"
2210       tokenType(tokenCount) = TOKTYPE_PLUS
2220       tokenValue$(tokenCount) = "+"
2230       tokenCount = tokenCount + 1
2240       curPos = curPos + 1
2250     CASE "-"
2260       tokenType(tokenCount) = TOKTYPE_MINUS
2270       tokenValue$(tokenCount) = "-"
2280       tokenCount = tokenCount + 1
2290       curPos = curPos + 1
2300     CASE "<"
2310       tokenType(tokenCount) = TOKTYPE_LESS
2320       tokenValue$(tokenCount) = "<"
2330       tokenCount = tokenCount + 1
2340       curPos = curPos + 1
2350     CASE ">"
2360       tokenType(tokenCount) = TOKTYPE_GREATER
2370       tokenValue$(tokenCount) = ">"
2380       tokenCount = tokenCount + 1
2390       curPos = curPos + 1
2400     CASE "="
2410       tokenType(tokenCount) = TOKTYPE_EQUAL
2420       tokenValue$(tokenCount) = "="
2430       tokenCount = tokenCount + 1
2440       curPos = curPos + 1
2450     CASE ";"
2460       tokenType(tokenCount) = TOKTYPE_SEMICOLON
2470       tokenValue$(tokenCount) = ";"
2480       tokenCount = tokenCount + 1
2490       curPos = curPos + 1
2500     CASE ELSE
2510       curPos = curPos + 1
2520   END SELECT
2530 LOOP

2540 tokenType(tokenCount) = TOKTYPE_EOF
2550 tokenValue$(tokenCount) = "EOF"
2560 RETURN

2600 ' ════════════════════════════════════════════════════════════════════════════════
2610 ' PARSER (RECURSIVE DESCENT)
2620 ' ════════════════════════════════════════════════════════════════════════════════

1500 REM Parse problem
1510 tokenPos = 0
1520 numConstraints = 0
1530 numVariables = 0
1540 varCount = 0
1550 
1560 ' Initialize arrays
1570 FOR i = 0 TO MAXCON - 1
1580   FOR j = 0 TO MAXVAR - 1
1590     constraintCoeff(i, j) = 0
1600   NEXT j
1610   constraintRhs(i) = 0
1620   constraintOp(i) = 0
1630 NEXT i
1640 
1650 FOR j = 0 TO MAXVAR - 1
1660   objectiveCoeff(j) = 0
1670 NEXT j
1680 
1690 ' Parse constraints until we find one with '=' (objective)
1700 DO WHILE tokenPos < tokenCount
1710   
1720   ' Parse constraint expression
1730   coeff = 0
1740   varIndex = -1
1750   
1760   DO WHILE tokenPos < tokenCount AND tokenType(tokenPos) <> TOKTYPE_SEMICOLON
1760     
1770     ' Get sign
1780     sign = 1
1790     IF tokenType(tokenPos) = TOKTYPE_PLUS THEN
1800       tokenPos = tokenPos + 1
1810     ELSEIF tokenType(tokenPos) = TOKTYPE_MINUS THEN
1820       sign = -1
1830       tokenPos = tokenPos + 1
1840     END IF
1850     
1860     ' Get coefficient (number)
1870     IF tokenPos < tokenCount AND tokenType(tokenPos) = TOKTYPE_NUMBER THEN
1880       coeff = VAL(tokenValue$(tokenPos)) * sign
1890       tokenPos = tokenPos + 1
1900     END IF
1910     
1920     ' Get variable name
1930     IF tokenPos < tokenCount AND tokenType(tokenPos) = TOKTYPE_IDENT THEN
1940       varName$ = tokenValue$(tokenPos)
1950       tokenPos = tokenPos + 1
1960       
1970       ' Find or add variable
1980       varIndex = -1
1990       FOR i = 0 TO varCount - 1
2000         IF varName$(i) = varName$ THEN
2010           varIndex = i
2020           EXIT FOR
2030         END IF
2040       NEXT i
2050       
2060       IF varIndex = -1 THEN
2070         varIndex = varCount
2080         varName$(varCount) = varName$
2090         varCount = varCount + 1
2100         numVariables = varCount
2110       END IF
2120       
2130       ' Store coefficient
2140       constraintCoeff(numConstraints, varIndex) = coeff
2150     END IF
2160   
2170   LOOP
2180   
2190   ' Get operator
2200   IF tokenPos < tokenCount THEN
2210     opType = 0
2220     SELECT CASE tokenType(tokenPos)
2230       CASE TOKTYPE_LESS
2240         opType = CONTYPE_LESS
2250       CASE TOKTYPE_GREATER
2260         opType = CONTYPE_GREATER
2270       CASE TOKTYPE_EQUAL
2280         opType = CONTYPE_EQUAL
2290     END SELECT
2300     tokenPos = tokenPos + 1
2310   END IF
2320   
2330   ' Get RHS
2340   rhs = 0
2350   IF tokenPos < tokenCount AND tokenType(tokenPos) = TOKTYPE_NUMBER THEN
2360     rhs = VAL(tokenValue$(tokenPos))
2370     tokenPos = tokenPos + 1
2380   END IF
2390   
2400   ' Skip semicolon
2410   IF tokenPos < tokenCount AND tokenType(tokenPos) = TOKTYPE_SEMICOLON THEN
2420     tokenPos = tokenPos + 1
2430   END IF
2440   
2450   ' Store constraint or objective
2460   IF opType = CONTYPE_EQUAL THEN
2470     ' This is the objective function
2480     FOR j = 0 TO numVariables - 1
2490       objectiveCoeff(j) = constraintCoeff(numConstraints, j)
2500     NEXT j
2510   ELSE
2520     ' This is a real constraint
2530     constraintOp(numConstraints) = opType
2540     constraintRhs(numConstraints) = rhs
2550     numConstraints = numConstraints + 1
2560   END IF
2570 
2580 LOOP

2590 PRINT "Parsed"; numVariables; "variables and"; numConstraints; "constraints"
2600 RETURN

2700 ' ════════════════════════════════════════════════════════════════════════════════
2710 ' SOLVER (SIMPLEX METHOD)
2720 ' ════════════════════════════════════════════════════════════════════════════════

2000 REM Build and solve tableau
2010 GOSUB 2100  ' Build tableau
2020 GOSUB 2200  ' Solve with simplex
2030 RETURN

2100 REM Build simplex tableau
2110 tableauRows = numConstraints + 1
2120 tableauCols = numVariables + numConstraints + 1

2130 ' Initialize tableau
2140 FOR i = 0 TO tableauRows - 1
2150   FOR j = 0 TO tableauCols - 1
2160     tableau(i, j) = 0
2170   NEXT j
2180 NEXT i

2190 ' Fill constraint rows
2200 FOR i = 0 TO numConstraints - 1
2210   ' Variable coefficients
2220   FOR j = 0 TO numVariables - 1
2230     tableau(i, j) = constraintCoeff(i, j)
2240   NEXT j
2250   
2260   ' Slack variables
2270   IF constraintOp(i) = CONTYPE_LESS THEN
2280     tableau(i, numVariables + i) = 1
2290   ELSEIF constraintOp(i) = CONTYPE_GREATER THEN
2300     tableau(i, numVariables + i) = -1
2310   END IF
2320   
2330   ' RHS
2340   tableau(i, tableauCols - 1) = constraintRhs(i)
2350   
2360   ' Track basis variables
2370   basisVar(i) = numVariables + i
2380 NEXT i

2390 ' Fill objective row (negated for maximization)
2400 FOR j = 0 TO numVariables - 1
2410   tableau(numConstraints, j) = -objectiveCoeff(j)
2420 NEXT j

2430 RETURN

2200 REM Simplex algorithm
2210 isOptimal = 0
2220 maxIterations = 100
2230 iteration = 0

2240 DO WHILE NOT isOptimal AND iteration < maxIterations
2250   iteration = iteration + 1
2260   
2270   ' Find entering variable (most negative cost)
2280   enterCol = -1
2290   minValue = 0
2300   FOR j = 0 TO numVariables + numConstraints - 1
2310     IF tableau(numConstraints, j) < minValue - EPSILON THEN
2320       minValue = tableau(numConstraints, j)
2320       enterCol = j
2330     END IF
2340   NEXT j
2350   
2360   IF enterCol = -1 THEN
2370     isOptimal = 1
2380     EXIT DO
2390   END IF
2400   
2410   ' Find leaving variable (minimum ratio test)
2420   leaveRow = -1
2430   minRatio = 999999
2440   FOR i = 0 TO numConstraints - 1
2450     IF tableau(i, enterCol) > EPSILON THEN
2460       ratio = tableau(i, tableauCols - 1) / tableau(i, enterCol)
2470       IF ratio >= 0 AND ratio < minRatio THEN
2480         minRatio = ratio
2490         leaveRow = i
2500       END IF
2510     END IF
2520   NEXT i
2530   
2540   IF leaveRow = -1 THEN
2550     PRINT "Problem is unbounded!"
2560     isOptimal = -1
2570     EXIT DO
2580   END IF
2590   
2600   ' Perform pivot
2610   GOSUB 2300
2620   
2630   ' Update basis
2640   basisVar(leaveRow) = enterCol
2650 
2660 LOOP

2670 RETURN

2300 REM Pivot operation
2310 pivotElement = tableau(leaveRow, enterCol)

2320 ' Normalize pivot row
2330 FOR j = 0 TO tableauCols - 1
2340   tableau(leaveRow, j) = tableau(leaveRow, j) / pivotElement
2350 NEXT j

2360 ' Eliminate entering column
2370 FOR i = 0 TO tableauRows - 1
2380   IF i <> leaveRow THEN
2390     factor = tableau(i, enterCol)
2400     FOR j = 0 TO tableauCols - 1
2410       tableau(i, j) = tableau(i, j) - factor * tableau(leaveRow, j)
2420     NEXT j
2430   END IF
2440 NEXT i

2450 RETURN

2700 ' ════════════════════════════════════════════════════════════════════════════════
2710 ' OUTPUT AND DISPLAY
2720 ' ════════════════════════════════════════════════════════════════════════════════

2400 REM Print results
2410 PRINT ""
2420 IF isOptimal = 1 THEN
2430   PRINT "╔════════════════════════════════════════╗"
2440   PRINT "║    ✓ SOLUTION FOUND                    ║"
2450   PRINT "╚════════════════════════════════════════╝"
2460   PRINT ""
2470   
2480   ' Extract optimal value
2490   optimalValue = -tableau(numConstraints, tableauCols - 1)
2500   PRINT "Optimal Value: "; optimalValue
2510   PRINT ""
2520   
2530   ' Extract solution
2540   PRINT "Variable Values:"
2550   FOR i = 0 TO numConstraints - 1
2560     varIdx = basisVar(i)
2570     IF varIdx < numVariables THEN
2580       PRINT "  "; varName$(varIdx); " = "; tableau(i, tableauCols - 1)
2590     END IF
2600   NEXT i
2610   
2620   PRINT ""
2630   PRINT "Press RETURN to continue..."
2640   INPUT dummy$
2650   
2660 ELSEIF isOptimal = -1 THEN
2670   PRINT "╔════════════════════════════════════════╗"
2680   PRINT "║    ⚠ PROBLEM IS UNBOUNDED             ║"
2690   PRINT "╚════════════════════════════════════════╝"
2700 ELSE
2710   PRINT "╔════════════════════════════════════════╗"
2720   PRINT "║    ✗ NO SOLUTION FOUND                 ║"
2730   PRINT "╚════════════════════════════════════════╝"
2740 END IF

2750 RETURN
