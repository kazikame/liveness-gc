eand: (define (and anx any)
              11: (let ((_var0 0: #t))
                       10: (let ((_var1 3: (eq? 1: anx
                                                2: _var0)))
                                9: (if 4: _var1
                                       5: any
                                       8: (let ((_var2 6: #f))
                                               7: _var2)))))

eor: (define (or orx ory)
             21: (let ((_var3 12: #t))
                      20: (let ((_var4 15: (eq? 13: orx
                                                14: _var3)))
                               19: (if 16: _var4
                                       17: orx
                                       18: ory))))

edivide: (define (divide a b)
                 29: (let ((_var5 22: 0))
                          28: (let ((_var6 26: (divide1 23: a
                                                        24: b
                                                        25: _var5)))
                                   27: _var6)))

edivide1: (define (divide1 a b i)
                  52: (let ((_var7 32: (< 30: a
                                          31: b)))
                           51: (if 33: _var7
                                   34: i
                                   50: (let ((_var8 37: (- 35: a
                                                           36: b)))
                                            49: (let ((_var10 38: 1))
                                                     48: (let ((_var11 41: (+ 39: i
                                                                              40: _var10)))
                                                              47: (let ((_var9 45: (divide1 42: _var8
                                                                                            43: b
                                                                                            44: _var11)))
                                                                       46: _var9)))))))

eNode: (define (Node left right)
               57: (let ((_var13 55: (cons 53: left
                                           54: right)))
                        56: _var13))

ePower2: (define (Power2 i)
                 84: (let ((_var14 58: 1))
                          83: (let ((_var15 61: (eq? 59: _var14
                                                     60: i)))
                                   82: (if 62: _var15
                                           65: (let ((_var16 63: 2))
                                                    64: _var16)
                                           81: (let ((_var17 66: 2))
                                                    80: (let ((_var19 67: 1))
                                                             79: (let ((_var20 70: (- 68: i
                                                                                      69: _var19)))
                                                                      78: (let ((_var21 72: (Power2 71: _var20)))
                                                                               77: (let ((_var18 75: (* 73: _var17
                                                                                                        74: _var21)))
                                                                                        76: _var18)))))))))

eTreeSize: (define (TreeSize i)
                   100: (let ((_var23 85: 1))
                             99: (let ((_var24 88: (+ 86: i
                                                      87: _var23)))
                                      98: (let ((_var25 90: (Power2 89: _var24)))
                                               97: (let ((_var27 91: 1))
                                                        96: (let ((_var26 94: (- 92: _var25
                                                                                 93: _var27)))
                                                                 95: _var26))))))

eNumIters: (define (NumIters i kStretchTreeDepth)
                   117: (let ((_var29 101: 2))
                             116: (let ((_var31 103: (TreeSize 102: kStretchTreeDepth)))
                                       115: (let ((_var33 105: (TreeSize 104: i)))
                                                 114: (let ((_var32 108: (divide 106: _var31
                                                                                 107: _var33)))
                                                           113: (let ((_var30 111: (* 109: _var29
                                                                                      110: _var32)))
                                                                     112: _var30))))))

eMakeTree: (define (MakeTree iDepth)
                   157: (let ((_var36 118: 0))
                             156: (let ((_var37 121: (eq? 119: _var36
                                                          120: iDepth)))
                                       155: (if 122: _var37
                                                131: (let ((_var38 123: '()))
                                                          130: (let ((_var40 124: '()))
                                                                    129: (let ((_var39 127: (Node 125: _var38
                                                                                                  126: _var40)))
                                                                              128: _var39)))
                                                154: (let ((_var42 132: 1))
                                                          153: (let ((_var43 135: (- 133: iDepth
                                                                                     134: _var42)))
                                                                    152: (let ((_var44 137: (MakeTree 136: _var43)))
                                                                              151: (let ((_var46 138: 1))
                                                                                        150: (let ((_var47 141: (- 139: iDepth
                                                                                                                   140: _var46)))
                                                                                                  149: (let ((_var48 143: (MakeTree 142: _var47)))
                                                                                                            148: (let ((_var45 146: (Node 144: _var44
                                                                                                                                          145: _var48)))
                                                                                                                      147: _var45)))))))))))

eBottomUp: (define (BottomUp depth)
                   161: (let ((_var50 159: (MakeTree 158: depth)))
                             160: _var50))

eLoopBottomUp: (define (LoopBottomUp i depth)
                       188: (let ((_var51 162: 0))
                                 187: (let ((_var52 165: (eq? 163: i
                                                              164: _var51)))
                                           186: (if 166: _var52
                                                    169: (let ((_var53 167: '()))
                                                              168: _var53)
                                                    185: (let ((tempTree 171: (MakeTree 170: depth)))
                                                              184: (let ((tempTree 172: '()))
                                                                        183: (let ((_var54 173: 1))
                                                                                  182: (let ((_var55 176: (- 174: i
                                                                                                             175: _var54)))
                                                                                            181: (let ((_var56 179: (LoopBottomUp 177: _var55
                                                                                                                                  178: depth)))
                                                                                                      180: _var56)))))))))

eTimeConstruction: (define (TimeConstruction depth kStretchTreeDepth)
                           197: (let ((iNumIters 191: (NumIters 189: depth
                                                                190: kStretchTreeDepth)))
                                     196: (let ((_var57 194: (LoopBottomUp 192: iNumIters
                                                                           193: depth)))
                                               195: _var57)))

eloopTimeConstruction: (define (loopTimeConstruction kMinTreeDepth kMaxTreeDepth kStretchTreeDepth)
                               222: (let ((_var58 200: (> 198: kMinTreeDepth
                                                          199: kMaxTreeDepth)))
                                         221: (if 201: _var58
                                                  204: (let ((_var59 202: '()))
                                                            203: _var59)
                                                  220: (let ((iter 207: (TimeConstruction 205: kMinTreeDepth
                                                                                          206: kStretchTreeDepth)))
                                                            219: (let ((_var60 208: 2))
                                                                      218: (let ((_var61 211: (+ 209: kMinTreeDepth
                                                                                                 210: _var60)))
                                                                                217: (let ((_var62 215: (loopTimeConstruction 212: _var61
                                                                                                                              213: kMaxTreeDepth
                                                                                                                              214: kStretchTreeDepth)))
                                                                                          216: _var62)))))))

ecreateArray: (define (createArray kArraySize i)
                      281: (let ((_var63 225: (eq? 223: i
                                                   224: kArraySize)))
                                280: (if 226: _var63
                                         235: (let ((_var64 227: '()))
                                                   234: (let ((_var66 228: '()))
                                                             233: (let ((_var65 231: (cons 229: _var64
                                                                                           230: _var66)))
                                                                       232: _var65)))
                                         279: (let ((_var68 236: 2))
                                                   278: (let ((_var69 239: (divide 237: kArraySize
                                                                                   238: _var68)))
                                                             277: (let ((_var70 242: (> 240: i
                                                                                        241: _var69)))
                                                                       276: (if 243: _var70
                                                                                260: (let ((_var71 244: '()))
                                                                                          259: (let ((_var73 245: 1))
                                                                                                    258: (let ((_var74 248: (+ 246: i
                                                                                                                               247: _var73)))
                                                                                                              257: (let ((_var75 251: (createArray 249: kArraySize
                                                                                                                                                   250: _var74)))
                                                                                                                        256: (let ((_var72 254: (cons 252: _var71
                                                                                                                                                      253: _var75)))
                                                                                                                                  255: _var72)))))
                                                                                275: (let ((_var77 261: 1))
                                                                                          274: (let ((_var78 264: (+ 262: i
                                                                                                                     263: _var77)))
                                                                                                    273: (let ((_var79 267: (createArray 265: kArraySize
                                                                                                                                         266: _var78)))
                                                                                                              272: (let ((_var80 270: (cons 268: i
                                                                                                                                            269: _var79)))
                                                                                                                        271: _var80)))))))))))

echeckArray: (define (checkArray array index)
                     307: (let ((_var81 282: 1))
                               306: (let ((_var82 285: (eq? 283: index
                                                            284: _var81)))
                                         305: (if 286: _var82
                                                  290: (let ((_var83 288: (car 287: array)))
                                                            289: _var83)
                                                  304: (let ((_var84 292: (cdr 291: array)))
                                                            303: (let ((_var86 293: 1))
                                                                      302: (let ((_var87 296: (- 294: index
                                                                                                 295: _var86)))
                                                                                301: (let ((_var85 299: (checkArray 297: _var84
                                                                                                                    298: _var87)))
                                                                                          300: _var85))))))))

emain: (define (main kStretchTreeDepth kLongLivedTreeDepth kArraySize kMinTreeDepth kMaxTreeDepth)
               365: (let ((tempTree 309: (MakeTree 308: kStretchTreeDepth)))
                         364: (let ((tempTree 310: '()))
                                   363: (let ((longLivedTree 312: (MakeTree 311: kLongLivedTreeDepth)))
                                             362: (let ((_var89 313: 1))
                                                       361: (let ((array 316: (createArray 314: kArraySize
                                                                                           315: _var89)))
                                                                 360: (let ((loopTC 320: (loopTimeConstruction 317: kMinTreeDepth
                                                                                                               318: kMaxTreeDepth
                                                                                                               319: kStretchTreeDepth)))
                                                                           359: (let ((_var91 322: (null? 321: longLivedTree)))
                                                                                     358: (let ((_var93 323: #f))
                                                                                               357: (let ((_var95 324: 1))
                                                                                                         356: (let ((_var96 327: (checkArray 325: array
                                                                                                                                             326: _var95)))
                                                                                                                   355: (let ((_var98 328: 1))
                                                                                                                             354: (let ((_var97 331: (eq? 329: _var96
                                                                                                                                                          330: _var98)))
                                                                                                                                       353: (let ((_var94 334: (eq? 332: _var93
                                                                                                                                                                    333: _var97)))
                                                                                                                                                 352: (let ((_var92 337: (or 335: _var91
                                                                                                                                                                             336: _var94)))
                                                                                                                                                           351: (if 338: _var92
                                                                                                                                                                    341: (let ((_var102 339: '()))
                                                                                                                                                                              340: _var102)
                                                                                                                                                                    350: (let ((_var103 342: 100))
                                                                                                                                                                              349: (let ((_var105 343: 100))
                                                                                                                                                                                        348: (let ((_var104 346: (cons 344: _var103
                                                                                                                                                                                                                       345: _var105)))
                                                                                                                                                                                                  347: _var104)))))))))))))))))))

emaketree-helper: (define (maketree-helper n k lefttree)
                          445: (let ((_var107 366: 0))
                                    444: (let ((_var108 369: (= 367: n
                                                                368: _var107)))
                                              443: (if 370: _var108
                                                       387: (let ((_var109 371: '()))
                                                                 386: (let ((_var111 372: '()))
                                                                           385: (let ((_var110 375: (cons 373: _var109
                                                                                                          374: _var111)))
                                                                                     384: (let ((_var113 378: (cons 376: lefttree
                                                                                                                    377: _var110)))
                                                                                               383: (let ((_var114 381: (cons 379: k
                                                                                                                              380: _var113)))
                                                                                                         382: _var114)))))
                                                       442: (let ((_var115 388: 1))
                                                                 441: (let ((_var116 391: (- 389: n
                                                                                             390: _var115)))
                                                                           440: (let ((l 395: (maketree-helper 392: _var116
                                                                                                               393: k
                                                                                                               394: lefttree)))
                                                                                     439: (let ((_var118 396: 1))
                                                                                               438: (let ((_var119 399: (- 397: n
                                                                                                                           398: _var118)))
                                                                                                         437: (let ((_var121 400: 1))
                                                                                                                   436: (let ((_var123 402: (car 401: l)))
                                                                                                                             435: (let ((_var122 405: (+ 403: _var121
                                                                                                                                                         404: _var123)))
                                                                                                                                       434: (let ((r 409: (maketree-helper 406: _var119
                                                                                                                                                                           407: _var122
                                                                                                                                                                           408: lefttree)))
                                                                                                                                                 433: (let ((_var126 410: 1))
                                                                                                                                                           432: (let ((_var128 412: (car 411: r)))
                                                                                                                                                                     431: (let ((_var127 415: (+ 413: _var126
                                                                                                                                                                                                 414: _var128)))
                                                                                                                                                                               430: (let ((_var131 416: '()))
                                                                                                                                                                                         429: (let ((_var132 419: (cons 417: r
                                                                                                                                                                                                                        418: _var131)))
                                                                                                                                                                                                   428: (let ((_var133 422: (cons 420: l
                                                                                                                                                                                                                                  421: _var132)))
                                                                                                                                                                                                             427: (let ((_var130 425: (cons 423: _var127
                                                                                                                                                                                                                                            424: _var133)))
                                                                                                                                                                                                                       426: _var130))))))))))))))))))))

emaketree: (define (maketree n)
                   498: (let ((_var135 446: 1))
                             497: (let ((_var136 449: (- 447: n
                                                         448: _var135)))
                                       496: (let ((_var138 450: 1))
                                                 495: (let ((_var140 451: '()))
                                                           494: (let ((rt-tree 455: (maketree-helper 452: _var136
                                                                                                     453: _var138
                                                                                                     454: _var140)))
                                                                     493: (let ((_var142 456: 1))
                                                                               492: (let ((_var143 459: (- 457: n
                                                                                                           458: _var142)))
                                                                                         491: (let ((_var145 460: 1))
                                                                                                   490: (let ((_var147 462: (car 461: rt-tree)))
                                                                                                             489: (let ((_var146 465: (+ 463: _var145
                                                                                                                                         464: _var147)))
                                                                                                                       488: (let ((left-tree 469: (maketree-helper 466: _var143
                                                                                                                                                                   467: _var146
                                                                                                                                                                   468: rt-tree)))
                                                                                                                                 487: (let ((_var150 470: 1))
                                                                                                                                           486: (let ((_var152 472: (car 471: left-tree)))
                                                                                                                                                     485: (let ((_var151 475: (+ 473: _var150
                                                                                                                                                                                 474: _var152)))
                                                                                                                                                               484: (let ((_var155 478: (cons 476: left-tree
                                                                                                                                                                                              477: rt-tree)))
                                                                                                                                                                         483: (let ((_var154 481: (cons 479: _var151
                                                                                                                                                                                                        480: _var155)))
                                                                                                                                                                                   482: _var154)))))))))))))))))

ezig: (define (zig t)
              574: (let ((_var157 499: 16))
                        573: (let ((_var159 500: 10))
                                  572: (let ((_var161 501: 5))
                                            571: (let ((_var163 502: 4))
                                                      570: (let ((_var165 503: 10))
                                                                569: (let ((v 509: (main 504: _var157
                                                                                         505: _var159
                                                                                         506: _var161
                                                                                         507: _var163
                                                                                         508: _var165)))
                                                                          568: (let ((_var167 511: (null? 510: t)))
                                                                                    567: (let ((_var169 513: (cdr 512: t)))
                                                                                              566: (let ((_var170 515: (car 514: _var169)))
                                                                                                        565: (let ((_var171 517: (null? 516: _var170)))
                                                                                                                  564: (let ((_var168 520: (or 518: _var167
                                                                                                                                               519: _var171)))
                                                                                                                            563: (if 521: _var168
                                                                                                                                     538: (let ((_var173 523: (car 522: t)))
                                                                                                                                               537: (let ((_var175 525: (car 524: v)))
                                                                                                                                                         536: (let ((_var174 528: (+ 526: _var173
                                                                                                                                                                                     527: _var175)))
                                                                                                                                                                   535: (let ((_var178 529: '()))
                                                                                                                                                                             534: (let ((_var177 532: (cons 530: _var174
                                                                                                                                                                                                            531: _var178)))
                                                                                                                                                                                       533: _var177)))))
                                                                                                                                     562: (let ((_var180 540: (car 539: t)))
                                                                                                                                               561: (let ((_var182 542: (car 541: v)))
                                                                                                                                                         560: (let ((_var181 545: (+ 543: _var180
                                                                                                                                                                                     544: _var182)))
                                                                                                                                                                   559: (let ((_var185 547: (cdr 546: t)))
                                                                                                                                                                             558: (let ((_var186 549: (car 548: _var185)))
                                                                                                                                                                                       557: (let ((_var187 551: (zag 550: _var186)))
                                                                                                                                                                                                 556: (let ((_var184 554: (cons 552: _var181
                                                                                                                                                                                                                                553: _var187)))
                                                                                                                                                                                                           555: _var184))))))))))))))))))))

ezag: (define (zag t)
              656: (let ((_var189 575: 16))
                        655: (let ((_var191 576: 10))
                                  654: (let ((_var193 577: 5))
                                            653: (let ((_var195 578: 4))
                                                      652: (let ((_var197 579: 10))
                                                                651: (let ((v 585: (main 580: _var189
                                                                                         581: _var191
                                                                                         582: _var193
                                                                                         583: _var195
                                                                                         584: _var197)))
                                                                          650: (let ((_var199 587: (null? 586: t)))
                                                                                    649: (let ((_var201 589: (cdr 588: t)))
                                                                                              648: (let ((_var202 591: (cdr 590: _var201)))
                                                                                                        647: (let ((_var203 593: (car 592: _var202)))
                                                                                                                  646: (let ((_var204 595: (null? 594: _var203)))
                                                                                                                            645: (let ((_var200 598: (or 596: _var199
                                                                                                                                                         597: _var204)))
                                                                                                                                      644: (if 599: _var200
                                                                                                                                               616: (let ((_var206 601: (car 600: t)))
                                                                                                                                                         615: (let ((_var208 603: (car 602: v)))
                                                                                                                                                                   614: (let ((_var207 606: (+ 604: _var206
                                                                                                                                                                                               605: _var208)))
                                                                                                                                                                             613: (let ((_var211 607: '()))
                                                                                                                                                                                       612: (let ((_var210 610: (cons 608: _var207
                                                                                                                                                                                                                      609: _var211)))
                                                                                                                                                                                                 611: _var210)))))
                                                                                                                                               643: (let ((_var213 618: (car 617: t)))
                                                                                                                                                         642: (let ((_var215 620: (car 619: v)))
                                                                                                                                                                   641: (let ((_var214 623: (+ 621: _var213
                                                                                                                                                                                               622: _var215)))
                                                                                                                                                                             640: (let ((_var218 625: (cdr 624: t)))
                                                                                                                                                                                       639: (let ((_var219 627: (cdr 626: _var218)))
                                                                                                                                                                                                 638: (let ((_var220 629: (car 628: _var219)))
                                                                                                                                                                                                           637: (let ((_var221 631: (zag 630: _var220)))
                                                                                                                                                                                                                     636: (let ((_var217 634: (cons 632: _var214
                                                                                                                                                                                                                                                    633: _var221)))
                                                                                                                                                                                                                               635: _var217))))))))))))))))))))))

665: (let ((_var223 657: 11))
          664: (let ((t 659: (maketree 658: _var223)))
                    663: (let ((_var225 661: (zig 660: t)))
                              662: _var225)))