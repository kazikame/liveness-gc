(define (or orx ory)
        (let ((_var0 #t))
             (let ((_var1 (eq? orx
                               _var0)))
                  (if _var1
                      orx
                      ory))))

(define (and anx any)
        (let ((_var2 #t))
             (let ((_var3 (eq? anx
                               _var2)))
                  (if _var3
                      any
                      (let ((_var4 #f))
                           _var4)))))

(define (append ax ay)
        (let ((_var5 (null? ax)))
             (if _var5
                 ay
                 (let ((_var6 (car ax)))
                      (let ((_var8 (cdr ax)))
                           (let ((_var9 (append _var8
                                                ay)))
                                (let ((_var7 (cons _var6
                                                   _var9)))
                                     _var7)))))))

(define (inResult x y result)
        (let ((_var11 (null? result)))
             (if _var11
                 (let ((_var12 0))
                      _var12)
                 (let ((posxy (car result)))
                      (let ((_var13 (car posxy)))
                           (let ((_var14 (eq? x
                                              _var13)))
                                (let ((_var16 (cdr posxy)))
                                     (let ((_var17 (eq? y
                                                        _var16)))
                                          (let ((_var15 (and _var14
                                                             _var17)))
                                               (if _var15
                                                   (let ((_var19 1))
                                                        _var19)
                                                   (let ((_var20 (cdr result)))
                                                        (let ((_var21 (inResult x
                                                                                y
                                                                                _var20)))
                                                             _var21))))))))))))

(define (check x y curlst boardsize)
        (let ((_var22 (> x
                         boardsize)))
             (let ((_var24 1))
                  (let ((_var25 (< x
                                   _var24)))
                       (let ((_var23 (or _var22
                                         _var25)))
                            (if _var23
                                (let ((_var27 '()))
                                     _var27)
                                (let ((_var28 (> y
                                                 boardsize)))
                                     (let ((_var30 1))
                                          (let ((_var31 (< y
                                                           _var30)))
                                               (let ((_var29 (or _var28
                                                                 _var31)))
                                                    (if _var29
                                                        (let ((_var33 '()))
                                                             _var33)
                                                        (let ((_var34 1))
                                                             (let ((_var36 (inResult x
                                                                                     y
                                                                                     curlst)))
                                                                  (let ((_var35 (eq? _var34
                                                                                     _var36)))
                                                                       (if _var35
                                                                           (let ((_var38 '()))
                                                                                _var38)
                                                                           (let ((_var39 (cons x
                                                                                               y)))
                                                                                (let ((_var41 '()))
                                                                                     (let ((_var40 (cons _var39
                                                                                                         _var41)))
                                                                                          (let ((_var43 (append curlst
                                                                                                                _var40)))
                                                                                               _var43)))))))))))))))))))

(define (join ax ay)
        (let ((_var44 (null? ax)))
             (if _var44
                 ay
                 (let ((_var45 (null? ay)))
                      (if _var45
                          (let ((_var46 '()))
                               (let ((_var47 (cons ax
                                                   _var46)))
                                    _var47))
                          (let ((_var48 (cons ax
                                              ay)))
                               _var48))))))

(define (generateMoves posx posy)
        (let ((_var49 1))
             (let ((_var50 (+ posx
                              _var49)))
                  (let ((_var52 2))
                       (let ((_var53 (+ posy
                                        _var52)))
                            (let ((mv1 (cons _var50
                                             _var53)))
                                 (let ((_var55 1))
                                      (let ((_var56 (+ posx
                                                       _var55)))
                                           (let ((_var58 2))
                                                (let ((_var59 (- posy
                                                                 _var58)))
                                                     (let ((mv2 (cons _var56
                                                                      _var59)))
                                                          (let ((_var61 2))
                                                               (let ((_var62 (+ posx
                                                                                _var61)))
                                                                    (let ((_var64 1))
                                                                         (let ((_var65 (+ posy
                                                                                          _var64)))
                                                                              (let ((mv3 (cons _var62
                                                                                               _var65)))
                                                                                   (let ((_var67 2))
                                                                                        (let ((_var68 (+ posx
                                                                                                         _var67)))
                                                                                             (let ((_var70 1))
                                                                                                  (let ((_var71 (- posy
                                                                                                                   _var70)))
                                                                                                       (let ((mv4 (cons _var68
                                                                                                                        _var71)))
                                                                                                            (let ((_var73 1))
                                                                                                                 (let ((_var74 (- posx
                                                                                                                                  _var73)))
                                                                                                                      (let ((_var76 2))
                                                                                                                           (let ((_var77 (+ posy
                                                                                                                                            _var76)))
                                                                                                                                (let ((mv5 (cons _var74
                                                                                                                                                 _var77)))
                                                                                                                                     (let ((_var79 1))
                                                                                                                                          (let ((_var80 (- posx
                                                                                                                                                           _var79)))
                                                                                                                                               (let ((_var82 2))
                                                                                                                                                    (let ((_var83 (- posy
                                                                                                                                                                     _var82)))
                                                                                                                                                         (let ((mv6 (cons _var80
                                                                                                                                                                          _var83)))
                                                                                                                                                              (let ((_var85 2))
                                                                                                                                                                   (let ((_var86 (- posx
                                                                                                                                                                                    _var85)))
                                                                                                                                                                        (let ((_var88 1))
                                                                                                                                                                             (let ((_var89 (+ posy
                                                                                                                                                                                              _var88)))
                                                                                                                                                                                  (let ((mv7 (cons _var86
                                                                                                                                                                                                   _var89)))
                                                                                                                                                                                       (let ((_var91 2))
                                                                                                                                                                                            (let ((_var92 (- posx
                                                                                                                                                                                                             _var91)))
                                                                                                                                                                                                 (let ((_var94 1))
                                                                                                                                                                                                      (let ((_var95 (- posy
                                                                                                                                                                                                                       _var94)))
                                                                                                                                                                                                           (let ((mv8 (cons _var92
                                                                                                                                                                                                                            _var95)))
                                                                                                                                                                                                                (let ((_var97 '()))
                                                                                                                                                                                                                     (let ((_var98 (cons mv8
                                                                                                                                                                                                                                         _var97)))
                                                                                                                                                                                                                          (let ((_var99 (cons mv7
                                                                                                                                                                                                                                              _var98)))
                                                                                                                                                                                                                               (let ((_var100 (cons mv6
                                                                                                                                                                                                                                                    _var99)))
                                                                                                                                                                                                                                    (let ((_var101 (cons mv5
                                                                                                                                                                                                                                                         _var100)))
                                                                                                                                                                                                                                         (let ((_var102 (cons mv4
                                                                                                                                                                                                                                                              _var101)))
                                                                                                                                                                                                                                              (let ((_var103 (cons mv3
                                                                                                                                                                                                                                                                   _var102)))
                                                                                                                                                                                                                                                   (let ((_var104 (cons mv2
                                                                                                                                                                                                                                                                        _var103)))
                                                                                                                                                                                                                                                        (let ((_var105 (cons mv1
                                                                                                                                                                                                                                                                             _var104)))
                                                                                                                                                                                                                                                             _var105))))))))))))))))))))))))))))))))))))))))))))))))))

(define (tries plst move bs)
        (let ((_var106 (car move)))
             (let ((_var108 (cdr move)))
                  (let ((_var107 (check _var106
                                        _var108
                                        plst
                                        bs)))
                       _var107))))

(define (try plst movelst bs newlst)
        (let ((_var110 (null? movelst)))
             (if _var110
                 newlst
                 (let ((_var111 (car movelst)))
                      (let ((n (tries plst
                                      _var111
                                      bs)))
                           (let ((nlst (join n
                                             newlst)))
                                (let ((_var113 (cdr movelst)))
                                     (let ((_var114 (try plst
                                                         _var113
                                                         bs
                                                         nlst)))
                                          _var114))))))))

(define (getLastMove plst)
        (let ((_var115 (cdr plst)))
             (let ((_var116 (null? _var115)))
                  (if _var116
                      (let ((_var117 (car plst)))
                           _var117)
                      (let ((_var118 (cdr plst)))
                           (let ((_var119 (getLastMove _var118)))
                                _var119))))))

(define (explore bs pathlst)
        (let ((lastmove (getLastMove pathlst)))
             (let ((_var120 (car lastmove)))
                  (let ((_var122 (cdr lastmove)))
                       (let ((moves (generateMoves _var120
                                                   _var122)))
                            (let ((_var124 '()))
                                 (let ((_var125 (try pathlst
                                                     moves
                                                     bs
                                                     _var124)))
                                      _var125)))))))

(define (nextStep bs result)
        (let ((_var126 (null? result)))
             (if _var126
                 (let ((_var127 '()))
                      _var127)
                 (let ((_var128 (car result)))
                      (let ((_var129 (explore bs
                                              _var128)))
                           (let ((_var131 (cdr result)))
                                (let ((_var132 (nextStep bs
                                                         _var131)))
                                     (let ((_var130 (append _var129
                                                            _var132)))
                                          _var130))))))))

(define (divide1 a b n op)
        (let ((_var134 (< a
                          b)))
             (if _var134
                 (let ((_var135 0))
                      (let ((_var136 (eq? op
                                          _var135)))
                           (if _var136
                               n
                               a)))
                 (let ((_var137 (- a
                                   b)))
                      (let ((_var139 1))
                           (let ((_var140 (+ _var139
                                             n)))
                                (let ((_var138 (divide1 _var137
                                                        b
                                                        _var140
                                                        op)))
                                     _var138)))))))

(define (divide a b)
        (let ((_var142 0))
             (let ((_var144 0))
                  (let ((_var143 (divide1 a
                                          b
                                          _var142
                                          _var144)))
                       _var143))))

(define (modr a b)
        (let ((_var146 0))
             (let ((_var148 1))
                  (let ((_var147 (divide1 a
                                          b
                                          _var146
                                          _var148)))
                       _var147))))

(define (getLength lst n)
        (let ((_var150 (null? lst)))
             (if _var150
                 n
                 (let ((_var151 (cdr lst)))
                      (let ((_var153 1))
                           (let ((_var154 (+ n
                                             _var153)))
                                (let ((_var152 (getLength _var151
                                                          _var154)))
                                     _var152)))))))

(define (splitHalf lst len)
        (let ((_var156 0))
             (let ((_var157 (eq? _var156
                                 len)))
                  (if _var157
                      lst
                      (let ((_var158 (cdr lst)))
                           (let ((_var160 1))
                                (let ((_var161 (- len
                                                  _var160)))
                                     (let ((_var159 (splitHalf _var158
                                                               _var161)))
                                          _var159))))))))

(define (filter lst)
        (let ((_var163 0))
             (let ((len (getLength lst
                                   _var163)))
                  (let ((_var165 2))
                       (let ((_var166 (divide len
                                              _var165)))
                            (let ((_var167 (splitHalf lst
                                                      _var166)))
                                 _var167))))))

(define (solve step sx sy bs)
        (let ((_var168 1))
             (let ((_var169 (eq? step
                                 _var168)))
                  (if _var169
                      (let ((_var170 (cons sx
                                           sy)))
                           (let ((_var172 '()))
                                (let ((_var171 (cons _var170
                                                     _var172)))
                                     (let ((_var175 '()))
                                          (let ((_var174 (cons _var171
                                                               _var175)))
                                               _var174)))))
                      (let ((_var177 1))
                           (let ((_var178 (- step
                                             _var177)))
                                (let ((ans (solve _var178
                                                  sx
                                                  sy
                                                  bs)))
                                     (let ((_var180 0))
                                          (let ((_var182 5))
                                               (let ((_var183 (modr step
                                                                    _var182)))
                                                    (let ((_var181 (eq? _var180
                                                                        _var183)))
                                                         (if _var181
                                                             (let ((_var185 (filter ans)))
                                                                  (let ((_var186 (nextStep bs
                                                                                           _var185)))
                                                                       _var186))
                                                             (let ((_var187 (nextStep bs
                                                                                      ans)))
                                                                  _var187)))))))))))))

(define (knightTour boardsize startposx startposy)
        (let ((_var188 (* boardsize
                          boardsize)))
             (let ((_var189 (solve _var188
                                   startposx
                                   startposy
                                   boardsize)))
                  _var189)))

(let ((_var190 5))
     (let ((_var192 3))
          (let ((_var194 3))
               (let ((_var191 (knightTour _var190
                                          _var192
                                          _var194)))
                    _var191))))