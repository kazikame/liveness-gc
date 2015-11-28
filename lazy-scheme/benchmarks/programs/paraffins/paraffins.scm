(define (remainder n k)
        (let ((_var0 (< n
                        k)))
             (if _var0
                 n
                 (let ((_var1 (- n
                                 k)))
                      (let ((_var2 (remainder _var1
                                              k)))
                           _var2)))))

(define (odd? n)
        (let ((_var3 2))
             (let ((_var4 (remainder n
                                     _var3)))
                  (let ((_var6 1))
                       (let ((_var5 (= _var4
                                       _var6)))
                            _var5)))))

(define (max m n)
        (let ((_var8 (> m
                        n)))
             (if _var8
                 m
                 n)))

(define (length l)
        (let ((_var9 (null? l)))
             (if _var9
                 (let ((_var10 0))
                      _var10)
                 (let ((_var11 1))
                      (let ((_var13 (cdr l)))
                           (let ((_var14 (length _var13)))
                                (let ((_var12 (+ _var11
                                                 _var14)))
                                     _var12)))))))

(define (quotient n k)
        (let ((_var16 (< n
                         k)))
             (if _var16
                 (let ((_var17 0))
                      _var17)
                 (let ((_var18 1))
                      (let ((_var20 (- n
                                       k)))
                           (let ((_var21 (quotient _var20
                                                   k)))
                                (let ((_var19 (+ _var18
                                                 _var21)))
                                     _var19)))))))

(define (make-vector k val)
        (let ((_var23 0))
             (let ((_var24 (= k
                              _var23)))
                  (if _var24
                      (let ((_var25 '()))
                           _var25)
                      (let ((_var26 1))
                           (let ((_var27 (- k
                                            _var26)))
                                (let ((_var28 (make-vector _var27
                                                           val)))
                                     (let ((_var29 (cons val
                                                         _var28)))
                                          _var29))))))))

(define (vector-ref v k)
        (let ((_var30 0))
             (let ((_var31 (= k
                              _var30)))
                  (if _var31
                      (let ((_var32 (car v)))
                           _var32)
                      (let ((_var33 (cdr v)))
                           (let ((_var35 1))
                                (let ((_var36 (- k
                                                 _var35)))
                                     (let ((_var34 (vector-ref _var33
                                                               _var36)))
                                          _var34))))))))

(define (vector-1 val)
        (let ((_var38 '()))
             (let ((_var39 (cons val
                                 _var38)))
                  _var39)))

(define (vector-2 val1 val2)
        (let ((_var40 (vector-1 val2)))
             (let ((_var41 (cons val1
                                 _var40)))
                  _var41)))

(define (vector-3 val1 val2 val3)
        (let ((_var42 (vector-2 val2
                                val3)))
             (let ((_var43 (cons val1
                                 _var42)))
                  _var43)))

(define (vector-4 val1 val2 val3 val4)
        (let ((_var44 (vector-3 val2
                                val3
                                val4)))
             (let ((_var45 (cons val1
                                 _var44)))
                  _var45)))

(define (vector-5 val1 val2 val3 val4 val5)
        (let ((_var46 (vector-4 val2
                                val3
                                val4
                                val5)))
             (let ((_var47 (cons val1
                                 _var46)))
                  _var47)))

(define (vector-set! vec i val)
        (let ((_var48 0))
             (let ((_var49 (= i
                              _var48)))
                  (if _var49
                      (let ((_var50 (cdr vec)))
                           (let ((_var51 (cons val
                                               _var50)))
                                _var51))
                      (let ((_var52 (car vec)))
                           (let ((_var54 (cdr vec)))
                                (let ((_var56 1))
                                     (let ((_var57 (- i
                                                      _var56)))
                                          (let ((_var55 (vector-set! _var54
                                                                     _var57
                                                                     val)))
                                               (let ((_var53 (cons _var52
                                                                   _var55)))
                                                    _var53))))))))))

(define (gen n)
        (let ((_var60 2))
             (let ((n/2 (quotient n
                                  _var60)))
                  (let ((_var62 1))
                       (let ((_var63 (+ n/2
                                        _var62)))
                            (let ((_var65 "H"))
                                 (let ((_var67 '()))
                                      (let ((_var66 (cons _var65
                                                          _var67)))
                                           (let ((radicals (make-vector _var63
                                                                        _var66)))
                                                (let ((_var70 1))
                                                     (let ((_var71 (gen-radicals _var70
                                                                                 n
                                                                                 radicals)))
                                                          _var71)))))))))))

(define (gen-radicals i n radicals)
        (let ((_var72 2))
             (let ((n/2 (quotient n
                                  _var72)))
                  (let ((_var74 (> i
                                   n/2)))
                       (if _var74
                           (let ((_var75 (bcp-generator n
                                                        radicals)))
                                (let ((_var77 (ccp-generator n
                                                             radicals)))
                                     (let ((_var76 (vector-2 _var75
                                                             _var77)))
                                          _var76)))
                           (let ((_var79 (rads-of-size i
                                                       radicals)))
                                (let ((new-rads (vector-set! radicals
                                                             i
                                                             _var79)))
                                     (let ((_var81 1))
                                          (let ((_var82 (+ _var81
                                                           i)))
                                               (let ((_var83 (gen-radicals _var82
                                                                           n
                                                                           new-rads)))
                                                    _var83))))))))))

(define (myloop4 rads1 rads2 rads3 lst)
        (let ((_var84 (null? rads3)))
             (if _var84
                 lst
                 (let ((_var85 "C"))
                      (let ((_var87 (car rads1)))
                           (let ((_var89 (car rads2)))
                                (let ((_var91 (car rads3)))
                                     (let ((_var86 (vector-4 _var85
                                                             _var87
                                                             _var89
                                                             _var91)))
                                          (let ((_var94 (cdr rads3)))
                                               (let ((_var95 (myloop4 rads1
                                                                      rads2
                                                                      _var94
                                                                      lst)))
                                                    (let ((_var93 (cons _var86
                                                                        _var95)))
                                                         _var93)))))))))))

(define (myloop3 nc2 nc3 radicals rads1 rads2 lst)
        (let ((_var97 (null? rads2)))
             (if _var97
                 lst
                 (let ((_var98 (= nc2
                                  nc3)))
                      (if _var98
                          (let ((_var102 (cdr rads2)))
                               (let ((_var103 (myloop3 nc2
                                                       nc3
                                                       radicals
                                                       rads1
                                                       _var102
                                                       lst)))
                                    (let ((_var100 (myloop4 rads1
                                                            rads2
                                                            rads2
                                                            _var103)))
                                         _var100)))
                          (let ((_var99 (vector-ref radicals
                                                    nc3)))
                               (let ((_var105 (cdr rads2)))
                                    (let ((_var106 (myloop3 nc2
                                                            nc3
                                                            radicals
                                                            rads1
                                                            _var105
                                                            lst)))
                                         (let ((_var101 (myloop4 rads1
                                                                 rads2
                                                                 _var99
                                                                 _var106)))
                                              _var101)))))))))

(define (myloop2 nc1 nc2 nc3 radicals rads1 lst)
        (let ((_var108 (null? rads1)))
             (if _var108
                 lst
                 (let ((_var109 (= nc1
                                   nc2)))
                      (if _var109
                          (let ((_var113 (cdr rads1)))
                               (let ((_var114 (myloop2 nc1
                                                       nc2
                                                       nc3
                                                       radicals
                                                       _var113
                                                       lst)))
                                    (let ((_var111 (myloop3 nc2
                                                            nc3
                                                            radicals
                                                            rads1
                                                            rads1
                                                            _var114)))
                                         _var111)))
                          (let ((_var110 (vector-ref radicals
                                                     nc2)))
                               (let ((_var116 (cdr rads1)))
                                    (let ((_var117 (myloop2 nc1
                                                            nc2
                                                            nc3
                                                            radicals
                                                            _var116
                                                            lst)))
                                         (let ((_var112 (myloop3 nc2
                                                                 nc3
                                                                 radicals
                                                                 rads1
                                                                 _var110
                                                                 _var117)))
                                              _var112)))))))))

(define (myloop1 radicals ps lst)
        (let ((_var119 (null? ps)))
             (if _var119
                 lst
                 (let ((p (car ps)))
                      (let ((_var120 0))
                           (let ((nc1 (vector-ref p
                                                  _var120)))
                                (let ((_var122 1))
                                     (let ((nc2 (vector-ref p
                                                            _var122)))
                                          (let ((_var124 2))
                                               (let ((nc3 (vector-ref p
                                                                      _var124)))
                                                    (let ((_var126 (vector-ref radicals
                                                                               nc1)))
                                                         (let ((_var128 (cdr ps)))
                                                              (let ((_var129 (myloop1 radicals
                                                                                      _var128
                                                                                      lst)))
                                                                   (let ((_var127 (myloop2 nc1
                                                                                           nc2
                                                                                           nc3
                                                                                           radicals
                                                                                           _var126
                                                                                           _var129)))
                                                                        _var127))))))))))))))

(define (rads-of-size n radicals)
        (let ((_var131 1))
             (let ((_var132 (- n
                               _var131)))
                  (let ((_var133 (three-partitions _var132)))
                       (let ((_var135 '()))
                            (let ((_var134 (myloop1 radicals
                                                    _var133
                                                    _var135)))
                                 _var134))))))

(define (bcp-loop1 rads1 lst)
        (let ((_var137 (null? rads1)))
             (if _var137
                 lst
                 (let ((_var138 (cdr rads1)))
                      (let ((_var139 (bcp-loop1 _var138
                                                lst)))
                           (let ((_var140 (bcp-loop2 rads1
                                                     rads1
                                                     _var139)))
                                _var140))))))

(define (bcp-loop2 rads1 rads2 lst)
        (let ((_var141 (null? rads2)))
             (if _var141
                 lst
                 (let ((_var142 "BCP"))
                      (let ((_var144 (car rads1)))
                           (let ((_var146 (car rads2)))
                                (let ((_var143 (vector-3 _var142
                                                         _var144
                                                         _var146)))
                                     (let ((_var149 (cdr rads2)))
                                          (let ((_var150 (bcp-loop2 rads1
                                                                    _var149
                                                                    lst)))
                                               (let ((_var148 (cons _var143
                                                                    _var150)))
                                                    _var148))))))))))

(define (bcp-generator j radicals)
        (let ((_var152 (odd? j)))
             (if _var152
                 (let ((_var153 '()))
                      _var153)
                 (let ((_var154 2))
                      (let ((_var155 (quotient j
                                               _var154)))
                           (let ((_var156 (vector-ref radicals
                                                      _var155)))
                                (let ((_var158 '()))
                                     (let ((_var157 (bcp-loop1 _var156
                                                               _var158)))
                                          _var157))))))))

(define (ccp-loop1 radicals ps lst)
        (let ((_var160 (null? ps)))
             (if _var160
                 lst
                 (let ((p (car ps)))
                      (let ((_var161 0))
                           (let ((nc1 (vector-ref p
                                                  _var161)))
                                (let ((_var163 1))
                                     (let ((nc2 (vector-ref p
                                                            _var163)))
                                          (let ((_var165 2))
                                               (let ((nc3 (vector-ref p
                                                                      _var165)))
                                                    (let ((_var167 3))
                                                         (let ((nc4 (vector-ref p
                                                                                _var167)))
                                                              (let ((_var169 (vector-ref radicals
                                                                                         nc1)))
                                                                   (let ((_var171 (cdr ps)))
                                                                        (let ((_var172 (ccp-loop1 radicals
                                                                                                  _var171
                                                                                                  lst)))
                                                                             (let ((_var170 (ccp-loop2 nc1
                                                                                                       nc2
                                                                                                       nc3
                                                                                                       nc4
                                                                                                       radicals
                                                                                                       _var169
                                                                                                       _var172)))
                                                                                  _var170))))))))))))))))

(define (ccp-loop2 nc1 nc2 nc3 nc4 radicals rads1 lst)
        (let ((_var174 (null? rads1)))
             (if _var174
                 lst
                 (let ((_var175 (= nc1
                                   nc2)))
                      (if _var175
                          (let ((_var179 (cdr rads1)))
                               (let ((_var180 (ccp-loop2 nc1
                                                         nc2
                                                         nc3
                                                         nc4
                                                         radicals
                                                         _var179
                                                         lst)))
                                    (let ((_var177 (ccp-loop3 nc2
                                                              nc3
                                                              nc4
                                                              radicals
                                                              rads1
                                                              rads1
                                                              _var180)))
                                         _var177)))
                          (let ((_var176 (vector-ref radicals
                                                     nc2)))
                               (let ((_var182 (cdr rads1)))
                                    (let ((_var183 (ccp-loop2 nc1
                                                              nc2
                                                              nc3
                                                              nc4
                                                              radicals
                                                              _var182
                                                              lst)))
                                         (let ((_var178 (ccp-loop3 nc2
                                                                   nc3
                                                                   nc4
                                                                   radicals
                                                                   rads1
                                                                   _var176
                                                                   _var183)))
                                              _var178)))))))))

(define (ccp-loop3 nc2 nc3 nc4 radicals rads1 rads2 lst)
        (let ((_var185 (null? rads2)))
             (if _var185
                 lst
                 (let ((_var186 (= nc2
                                   nc3)))
                      (if _var186
                          (let ((_var190 (cdr rads2)))
                               (let ((_var191 (ccp-loop3 nc2
                                                         nc3
                                                         nc4
                                                         radicals
                                                         rads1
                                                         _var190
                                                         lst)))
                                    (let ((_var188 (ccp-loop4 nc3
                                                              nc4
                                                              radicals
                                                              rads1
                                                              rads2
                                                              rads2
                                                              _var191)))
                                         _var188)))
                          (let ((_var187 (vector-ref radicals
                                                     nc3)))
                               (let ((_var193 (cdr rads2)))
                                    (let ((_var194 (ccp-loop3 nc2
                                                              nc3
                                                              nc4
                                                              radicals
                                                              rads1
                                                              _var193
                                                              lst)))
                                         (let ((_var189 (ccp-loop4 nc3
                                                                   nc4
                                                                   radicals
                                                                   rads1
                                                                   rads2
                                                                   _var187
                                                                   _var194)))
                                              _var189)))))))))

(define (ccp-loop4 nc3 nc4 radicals rads1 rads2 rads3 lst)
        (let ((_var196 (null? rads3)))
             (if _var196
                 lst
                 (let ((_var197 (= nc3
                                   nc4)))
                      (if _var197
                          (let ((_var201 (cdr rads3)))
                               (let ((_var202 (ccp-loop4 nc3
                                                         nc4
                                                         radicals
                                                         rads1
                                                         rads2
                                                         _var201
                                                         lst)))
                                    (let ((_var199 (ccp-loop5 rads1
                                                              rads2
                                                              rads3
                                                              rads3
                                                              _var202)))
                                         _var199)))
                          (let ((_var198 (vector-ref radicals
                                                     nc4)))
                               (let ((_var204 (cdr rads3)))
                                    (let ((_var205 (ccp-loop4 nc3
                                                              nc4
                                                              radicals
                                                              rads1
                                                              rads2
                                                              _var204
                                                              lst)))
                                         (let ((_var200 (ccp-loop5 rads1
                                                                   rads2
                                                                   rads3
                                                                   _var198
                                                                   _var205)))
                                              _var200)))))))))

(define (ccp-loop5 rads1 rads2 rads3 rads4 lst)
        (let ((_var207 (null? rads4)))
             (if _var207
                 lst
                 (let ((_var208 "CCP"))
                      (let ((_var210 (car rads1)))
                           (let ((_var212 (car rads2)))
                                (let ((_var214 (car rads3)))
                                     (let ((_var216 (car rads4)))
                                          (let ((_var209 (vector-5 _var208
                                                                   _var210
                                                                   _var212
                                                                   _var214
                                                                   _var216)))
                                               (let ((_var219 (cdr rads4)))
                                                    (let ((_var220 (ccp-loop5 rads1
                                                                              rads2
                                                                              rads3
                                                                              _var219
                                                                              lst)))
                                                         (let ((_var218 (cons _var209
                                                                              _var220)))
                                                              _var218))))))))))))

(define (ccp-generator j radicals)
        (let ((_var222 1))
             (let ((_var223 (- j
                               _var222)))
                  (let ((_var224 (four-partitions _var223)))
                       (let ((_var226 '()))
                            (let ((_var225 (ccp-loop1 radicals
                                                      _var224
                                                      _var226)))
                                 _var225))))))

(define (tp-loop2 m nc1 lst nc2)
        (let ((_var228 (< nc2
                          nc1)))
             (if _var228
                 (let ((_var229 1))
                      (let ((_var230 (- nc1
                                        _var229)))
                           (let ((_var231 (tp-loop1 m
                                                    lst
                                                    _var230)))
                                _var231)))
                 (let ((_var232 (+ nc1
                                   nc2)))
                      (let ((_var233 (- m
                                        _var232)))
                           (let ((_var234 (vector-3 nc1
                                                    nc2
                                                    _var233)))
                                (let ((_var235 (cons _var234
                                                     lst)))
                                     (let ((_var237 1))
                                          (let ((_var238 (- nc2
                                                            _var237)))
                                               (let ((_var236 (tp-loop2 m
                                                                        nc1
                                                                        _var235
                                                                        _var238)))
                                                    _var236))))))))))

(define (tp-loop1 m lst nc1)
        (let ((_var240 0))
             (let ((_var241 (< nc1
                               _var240)))
                  (if _var241
                      lst
                      (let ((_var242 (- m
                                        nc1)))
                           (let ((_var244 2))
                                (let ((_var243 (quotient _var242
                                                         _var244)))
                                     (let ((_var246 (tp-loop2 m
                                                              nc1
                                                              lst
                                                              _var243)))
                                          _var246))))))))

(define (three-partitions m)
        (let ((_var247 '()))
             (let ((_var249 3))
                  (let ((_var250 (quotient m
                                           _var249)))
                       (let ((_var248 (tp-loop1 m
                                                _var247
                                                _var250)))
                            _var248)))))

(define (fp-loop3 m nc1 nc2 start lst nc3)
        (let ((_var252 (< nc3
                          start)))
             (if _var252
                 (let ((_var253 1))
                      (let ((_var254 (- nc2
                                        _var253)))
                           (let ((_var255 (fp-loop2 m
                                                    nc1
                                                    lst
                                                    _var254)))
                                _var255)))
                 (let ((_var256 (+ nc2
                                   nc3)))
                      (let ((_var257 (+ nc1
                                        _var256)))
                           (let ((_var258 (- m
                                             _var257)))
                                (let ((_var259 (vector-4 nc1
                                                         nc2
                                                         nc3
                                                         _var258)))
                                     (let ((_var260 (cons _var259
                                                          lst)))
                                          (let ((_var262 1))
                                               (let ((_var263 (- nc3
                                                                 _var262)))
                                                    (let ((_var261 (fp-loop3 m
                                                                             nc1
                                                                             nc2
                                                                             start
                                                                             _var260
                                                                             _var263)))
                                                         _var261)))))))))))

(define (fp-loop2 m nc1 lst nc2)
        (let ((_var265 (< nc2
                          nc1)))
             (if _var265
                 (let ((_var266 1))
                      (let ((_var267 (- nc1
                                        _var266)))
                           (let ((_var268 (fp-loop1 m
                                                    lst
                                                    _var267)))
                                _var268)))
                 (let ((_var269 1))
                      (let ((_var270 (+ m
                                        _var269)))
                           (let ((_var272 2))
                                (let ((_var271 (quotient _var270
                                                         _var272)))
                                     (let ((_var275 (+ nc1
                                                       nc2)))
                                          (let ((_var274 (- _var271
                                                            _var275)))
                                               (let ((start (max nc2
                                                                 _var274)))
                                                    (let ((_var278 (+ nc1
                                                                      nc2)))
                                                         (let ((_var279 (- m
                                                                           _var278)))
                                                              (let ((_var281 2))
                                                                   (let ((_var280 (quotient _var279
                                                                                            _var281)))
                                                                        (let ((_var283 (fp-loop3 m
                                                                                                 nc1
                                                                                                 nc2
                                                                                                 start
                                                                                                 lst
                                                                                                 _var280)))
                                                                             _var283)))))))))))))))

(define (fp-loop1 m lst nc1)
        (let ((_var284 0))
             (let ((_var285 (< nc1
                               _var284)))
                  (if _var285
                      lst
                      (let ((_var286 (- m
                                        nc1)))
                           (let ((_var288 3))
                                (let ((_var287 (quotient _var286
                                                         _var288)))
                                     (let ((_var290 (fp-loop2 m
                                                              nc1
                                                              lst
                                                              _var287)))
                                          _var290))))))))

(define (four-partitions m)
        (let ((_var291 '()))
             (let ((_var293 4))
                  (let ((_var294 (quotient m
                                           _var293)))
                       (let ((_var292 (fp-loop1 m
                                                _var291
                                                _var294)))
                            _var292)))))

(define (nb n)
        (let ((x (gen n)))
             (let ((_var296 0))
                  (let ((_var297 (vector-ref x
                                             _var296)))
                       (let ((_var298 (length _var297)))
                            (let ((_var300 1))
                                 (let ((_var301 (vector-ref x
                                                            _var300)))
                                      (let ((_var302 (length _var301)))
                                           (let ((_var299 (+ _var298
                                                             _var302)))
                                                _var299)))))))))

(let ((_var304 12))
     (let ((_var305 (nb _var304)))
          _var305))
