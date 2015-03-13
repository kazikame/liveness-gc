(define (isEqual lst1 lst2)
        (let ((_var0 (null? lst1)))
             (if _var0
                 (let ((_var1 (null? lst2)))
                      (if _var1
                          (let ((_var2 #t))
                               _var2)
                          (let ((_var3 #f))
                               _var3)))
                 (let ((_var4 (null? lst2)))
                      (if _var4
                          (let ((_var5 (null? lst1)))
                               (if _var5
                                   (let ((_var6 #t))
                                        _var6)
                                   (let ((_var7 #f))
                                        _var7)))
                          (let ((_var8 (car lst1)))
                               (let ((_var10 (car lst2)))
                                    (let ((_var9 (eq? _var8
                                                      _var10)))
                                         (if _var9
                                             (let ((_var12 (cdr lst1)))
                                                  (let ((_var14 (cdr lst2)))
                                                       (let ((_var13 (isEqual _var12
                                                                              _var14)))
                                                            _var13)))
                                             (let ((_var16 #f))
                                                  _var16))))))))))

(define (mkVar s)
        (let ((_var17 "Var"))
             (let ((_var19 '()))
                  (let ((_var20 (cons s
                                      _var19)))
                       (let ((_var18 (cons _var17
                                           _var20)))
                            _var18)))))

(define (mkCon i)
        (let ((_var22 "Con"))
             (let ((_var24 '()))
                  (let ((_var25 (cons i
                                      _var24)))
                       (let ((_var23 (cons _var22
                                           _var25)))
                            _var23)))))

(define (bkCon c)
        (let ((_var27 (cdr c)))
             (let ((_var28 (car _var27)))
                  _var28)))

(define (mkIncr)
        (let ((_var29 "Incr"))
             (let ((_var31 '()))
                  (let ((_var30 (cons _var29
                                      _var31)))
                       _var30))))

(define (mkAdd t1 t2)
        (let ((_var33 "Add"))
             (let ((_var35 '()))
                  (let ((_var36 (cons t2
                                      _var35)))
                       (let ((_var37 (cons t1
                                           _var36)))
                            (let ((_var34 (cons _var33
                                                _var37)))
                                 _var34))))))

(define (mkLam s t)
        (let ((_var39 "Lam"))
             (let ((_var41 '()))
                  (let ((_var42 (cons t
                                      _var41)))
                       (let ((_var43 (cons s
                                           _var42)))
                            (let ((_var40 (cons _var39
                                                _var43)))
                                 _var40))))))

(define (mkApp t1 t2)
        (let ((_var45 "App"))
             (let ((_var47 '()))
                  (let ((_var48 (cons t2
                                      _var47)))
                       (let ((_var49 (cons t1
                                           _var48)))
                            (let ((_var46 (cons _var45
                                                _var49)))
                                 _var46))))))

(define (mkIfZero t1 t2 t3)
        (let ((_var51 "IfZero"))
             (let ((_var53 '()))
                  (let ((_var54 (cons t3
                                      _var53)))
                       (let ((_var55 (cons t2
                                           _var54)))
                            (let ((_var56 (cons t1
                                                _var55)))
                                 (let ((_var52 (cons _var51
                                                     _var56)))
                                      _var52)))))))

(define (mkThunk t e)
        (let ((_var58 "Thunk"))
             (let ((_var60 '()))
                  (let ((_var61 (cons e
                                      _var60)))
                       (let ((_var62 (cons t
                                           _var61)))
                            (let ((_var59 (cons _var58
                                                _var62)))
                                 _var59))))))

(define (mkEnvPair s t)
        (let ((_var64 '()))
             (let ((_var65 (cons t
                                 _var64)))
                  (let ((_var66 (cons s
                                      _var65)))
                       _var66))))

(define (addToEnv e env)
        (let ((_var67 (cons e
                            env)))
             _var67))

(define (lookup s v)
        (let ((_var68 (null? s)))
             (if _var68
                 (let ((_var69 "error1-unknown-variable"))
                      (let ((_var71 1))
                           (let ((_var70 (cons _var69
                                               _var71)))
                                _var70)))
                 (let ((elt (car s)))
                      (let ((_var73 (car elt)))
                           (let ((_var74 (eq? v
                                              _var73)))
                                (if _var74
                                    (let ((_var75 (cdr elt)))
                                         (let ((_var76 (car _var75)))
                                              _var76))
                                    (let ((_var77 (cdr s)))
                                         (let ((_var78 (lookup _var77
                                                               v)))
                                              _var78)))))))))

(define (simpleEval env p)
        (let ((tag (car p)))
             (let ((_var79 "Var"))
                  (let ((_var80 (eq? _var79
                                     tag)))
                       (if _var80
                           (let ((_var81 (cdr p)))
                                (let ((_var82 (car _var81)))
                                     (let ((_var83 (lookup env
                                                           _var82)))
                                          (let ((_var84 (simpleEval env
                                                                    _var83)))
                                               _var84))))
                           (let ((_var85 "Add"))
                                (let ((_var86 (eq? _var85
                                                   tag)))
                                     (if _var86
                                         (let ((_var87 (cdr p)))
                                              (let ((_var88 (car _var87)))
                                                   (let ((u (simpleEval env
                                                                        _var88)))
                                                        (let ((_var90 (cdr p)))
                                                             (let ((_var91 (cdr _var90)))
                                                                  (let ((_var92 (car _var91)))
                                                                       (let ((v (simpleEval env
                                                                                            _var92)))
                                                                            (let ((_var94 (bkCon u)))
                                                                                 (let ((_var96 (bkCon v)))
                                                                                      (let ((_var95 (+ _var94
                                                                                                       _var96)))
                                                                                           (let ((_var98 (mkCon _var95)))
                                                                                                _var98)))))))))))
                                         (let ((_var99 "Thunk"))
                                              (let ((_var100 (eq? _var99
                                                                  tag)))
                                                   (if _var100
                                                       (let ((_var101 (cdr p)))
                                                            (let ((_var102 (cdr _var101)))
                                                                 (let ((_var103 (car _var102)))
                                                                      (let ((_var105 (cdr p)))
                                                                           (let ((_var106 (car _var105)))
                                                                                (let ((_var104 (simpleEval _var103
                                                                                                           _var106)))
                                                                                     _var104))))))
                                                       (let ((_var108 "Lam"))
                                                            (let ((_var109 (eq? _var108
                                                                                tag)))
                                                                 (if _var109
                                                                     (let ((_var110 (mkThunk p
                                                                                             env)))
                                                                          _var110)
                                                                     (let ((_var111 "App"))
                                                                          (let ((_var112 (eq? _var111
                                                                                              tag)))
                                                                               (if _var112
                                                                                   (let ((_var113 (cdr p)))
                                                                                        (let ((_var114 (car _var113)))
                                                                                             (let ((u (simpleEval env
                                                                                                                  _var114)))
                                                                                                  (let ((_var116 (cdr p)))
                                                                                                       (let ((_var117 (cdr _var116)))
                                                                                                            (let ((_var118 (car _var117)))
                                                                                                                 (let ((_var119 (simpleApply env
                                                                                                                                             u
                                                                                                                                             _var118)))
                                                                                                                      _var119)))))))
                                                                                   (let ((_var120 "IfZero"))
                                                                                        (let ((_var121 (eq? _var120
                                                                                                            tag)))
                                                                                             (if _var121
                                                                                                 (let ((_var122 (cdr p)))
                                                                                                      (let ((_var123 (car _var122)))
                                                                                                           (let ((val (simpleEval env
                                                                                                                                  _var123)))
                                                                                                                (let ((_var125 0))
                                                                                                                     (let ((_var126 (mkCon _var125)))
                                                                                                                          (let ((_var127 (isEqual _var126
                                                                                                                                                  val)))
                                                                                                                               (if _var127
                                                                                                                                   (let ((_var128 (cdr p)))
                                                                                                                                        (let ((_var129 (cdr _var128)))
                                                                                                                                             (let ((_var130 (car _var129)))
                                                                                                                                                  (let ((_var131 (simpleEval env
                                                                                                                                                                             _var130)))
                                                                                                                                                       _var131))))
                                                                                                                                   (let ((_var132 (cdr p)))
                                                                                                                                        (let ((_var133 (cdr _var132)))
                                                                                                                                             (let ((_var134 (cdr _var133)))
                                                                                                                                                  (let ((_var135 (car _var134)))
                                                                                                                                                       (let ((_var136 (simpleEval env
                                                                                                                                                                                  _var135)))
                                                                                                                                                            _var136))))))))))))
                                                                                                 (let ((_var137 "Con"))
                                                                                                      (let ((_var138 (eq? _var137
                                                                                                                          tag)))
                                                                                                           (if _var138
                                                                                                               p
                                                                                                               (let ((_var139 "Incr"))
                                                                                                                    (let ((_var140 (eq? _var139
                                                                                                                                        tag)))
                                                                                                                         (if _var140
                                                                                                                             (let ((_var141 0))
                                                                                                                                  (let ((_var142 (mkCon _var141)))
                                                                                                                                       _var142))
                                                                                                                             (let ((_var143 "error2-in-simpleEval"))
                                                                                                                                  (let ((_var144 (cons _var143
                                                                                                                                                       p)))
                                                                                                                                       _var144))))))))))))))))))))))))))))

(define (simpleApply env t a)
        (let ((tag (car t)))
             (let ((_var145 "Thunk"))
                  (let ((_var146 (eq? _var145
                                      tag)))
                       (if _var146
                           (let ((_var147 (cdr t)))
                                (let ((_var148 (car _var147)))
                                     (let ((_var149 (cdr _var148)))
                                          (let ((x (car _var149)))
                                               (let ((_var151 (cdr t)))
                                                    (let ((_var152 (car _var151)))
                                                         (let ((_var153 (cdr _var152)))
                                                              (let ((_var154 (cdr _var153)))
                                                                   (let ((b (car _var154)))
                                                                        (let ((_var156 (cdr t)))
                                                                             (let ((_var157 (cdr _var156)))
                                                                                  (let ((e (car _var157)))
                                                                                       (let ((_var159 (mkThunk a
                                                                                                               env)))
                                                                                            (let ((_var160 (mkEnvPair x
                                                                                                                      _var159)))
                                                                                                 (let ((env2 (addToEnv _var160
                                                                                                                       e)))
                                                                                                      (let ((_var162 (simpleEval env2
                                                                                                                                 b)))
                                                                                                           _var162))))))))))))))))
                           (let ((_var163 "error3-cannot-simpleApply"))
                                (let ((_var165 3))
                                     (let ((_var164 (cons _var163
                                                          _var165)))
                                          _var164))))))))

(define (simpleMain arg)
        (let ((_var167 '()))
             (let ((_var169 (pgm arg)))
                  (let ((_var168 (simpleEval _var167
                                             _var169)))
                       _var168))))

(define (pgm arg)
        (let ((_var171 (sum0)))
             (let ((_var173 (mkCon arg)))
                  (let ((_var172 (mkApp _var171
                                        _var173)))
                       _var172))))

(define (nMinus1)
        (let ((_var175 "n"))
             (let ((_var176 (mkVar _var175)))
                  (let ((_var178 -1))
                       (let ((_var179 (mkCon _var178)))
                            (let ((_var177 (mkAdd _var176
                                                  _var179)))
                                 _var177))))))

(define (partialSum0)
        (let ((_var181 "sum"))
             (let ((_var183 "n"))
                  (let ((_var185 "n"))
                       (let ((_var186 (mkVar _var185)))
                            (let ((_var188 0))
                                 (let ((_var189 (mkCon _var188)))
                                      (let ((_var191 "n"))
                                           (let ((_var192 (mkVar _var191)))
                                                (let ((_var194 "sum"))
                                                     (let ((_var195 (mkVar _var194)))
                                                          (let ((_var197 (nMinus1)))
                                                               (let ((_var196 (mkApp _var195
                                                                                     _var197)))
                                                                    (let ((_var193 (mkAdd _var192
                                                                                          _var196)))
                                                                         (let ((_var187 (mkIfZero _var186
                                                                                                  _var189
                                                                                                  _var193)))
                                                                              (let ((_var184 (mkLam _var183
                                                                                                    _var187)))
                                                                                   (let ((_var182 (mkLam _var181
                                                                                                         _var184)))
                                                                                        _var182)))))))))))))))))

(define (lfxx)
        (let ((_var203 "x"))
             (let ((_var205 "F"))
                  (let ((_var206 (mkVar _var205)))
                       (let ((_var208 "x"))
                            (let ((_var209 (mkVar _var208)))
                                 (let ((_var211 "x"))
                                      (let ((_var212 (mkVar _var211)))
                                           (let ((_var210 (mkApp _var209
                                                                 _var212)))
                                                (let ((_var207 (mkApp _var206
                                                                      _var210)))
                                                     (let ((_var204 (mkLam _var203
                                                                           _var207)))
                                                          _var204)))))))))))

(define (fix)
        (let ((_var216 "F"))
             (let ((_var218 (lfxx)))
                  (let ((_var220 (lfxx)))
                       (let ((_var219 (mkApp _var218
                                             _var220)))
                            (let ((_var217 (mkLam _var216
                                                  _var219)))
                                 _var217))))))

(define (sum0)
        (let ((_var223 (fix)))
             (let ((_var225 (partialSum0)))
                  (let ((_var224 (mkApp _var223
                                        _var225)))
                       _var224))))

(let ((_var227 32))
     (let ((_var228 (simpleMain _var227)))
          _var228))