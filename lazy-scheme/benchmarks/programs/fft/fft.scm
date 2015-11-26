(define (or a b)
        (if a
            a
            b))

(define (factorial n)
        (let ((_var0 0))
             (let ((_var1 (= n
                             _var0)))
                  (if _var1
                      (let ((_var2 1))
                           _var2)
                      (let ((_var3 1))
                           (let ((_var4 (- n
                                           _var3)))
                                (let ((_var5 (factorial _var4)))
                                     (let ((_var6 (* n
                                                     _var5)))
                                          _var6))))))))

(define (cos x)
        (let ((sqrx (* x
                       x)))
             (let ((fourx (* sqrx
                             sqrx)))
                  (let ((_var7 1))
                       (let ((_var9 2))
                            (let ((_var10 (factorial _var9)))
                                 (let ((_var11 (divide sqrx
                                                       _var10)))
                                      (let ((_var13 4))
                                           (let ((_var14 (factorial _var13)))
                                                (let ((_var15 (divide fourx
                                                                      _var14)))
                                                     (let ((_var12 (- _var11
                                                                      _var15)))
                                                          (let ((_var8 (- _var7
                                                                          _var12)))
                                                               _var8))))))))))))

(define (sin x)
        (let ((sqrx (* x
                       x)))
             (let ((cubex (* sqrx
                             x)))
                  (let ((fivex (* cubex
                                  sqrx)))
                       (let ((_var18 3))
                            (let ((_var19 (factorial _var18)))
                                 (let ((_var20 (divide cubex
                                                       _var19)))
                                      (let ((_var22 5))
                                           (let ((_var23 (factorial _var22)))
                                                (let ((_var24 (divide fivex
                                                                      _var23)))
                                                     (let ((_var21 (- _var20
                                                                      _var24)))
                                                          (let ((_var26 (- x
                                                                           _var21)))
                                                               _var26))))))))))))

(define (good-enough? x y)
        (let ((_var27 1))
             (let ((_var29 (* x
                              x)))
                  (let ((_var30 (- _var29
                                   y)))
                       (let ((_var31 (abs _var30)))
                            (let ((_var28 (> _var27
                                             _var31)))
                                 _var28))))))

(define (better-guess a b)
        (let ((_var33 (* a
                         a)))
             (let ((diff (- _var33
                            b)))
                  (let ((_var35 0))
                       (let ((_var36 (> _var35
                                        diff)))
                            (if _var36
                                (let ((_var37 1))
                                     (let ((_var38 (+ a
                                                      _var37)))
                                          _var38))
                                (let ((_var39 1))
                                     (let ((_var40 (- a
                                                      _var39)))
                                          _var40))))))))

(define (sqrt-prime guess x)
        (let ((_var41 (good-enough? guess
                                    x)))
             (if _var41
                 guess
                 (let ((_var42 (better-guess guess
                                             x)))
                      (let ((_var43 (sqrt-prime _var42
                                                x)))
                           _var43)))))

(define (abs x)
        (let ((_var44 0))
             (let ((_var45 (> x
                              _var44)))
                  (if _var45
                      x
                      (let ((_var46 -1))
                           (let ((_var47 (* _var46
                                            x)))
                                _var47))))))

(define (sqrt x)
        (let ((_var48 2))
             (let ((_var49 (divide x
                                   _var48)))
                  _var49)))

(define (makeComplex1 lst1 lst2)
        (let ((_var50 (null? lst1)))
             (if _var50
                 lst1
                 (let ((_var51 (car lst1)))
                      (let ((_var53 (car lst2)))
                           (let ((_var52 (cons _var51
                                               _var53)))
                                (let ((_var56 (cdr lst1)))
                                     (let ((_var58 (cdr lst2)))
                                          (let ((_var57 (makeComplex1 _var56
                                                                      _var58)))
                                               (let ((_var55 (cons _var52
                                                                   _var57)))
                                                    _var55))))))))))

(define (makeComplex2 i n)
        (let ((_var61 (eq? i
                           n)))
             (if _var61
                 (let ((_var62 '()))
                      _var62)
                 (let ((_var63 1))
                      (let ((_var64 (* _var63
                                       i)))
                           (let ((_var66 0))
                                (let ((num (cons _var64
                                                 _var66)))
                                     (let ((_var68 1))
                                          (let ((_var69 (+ i
                                                           _var68)))
                                               (let ((_var70 (makeComplex2 _var69
                                                                           n)))
                                                    (let ((_var71 (cons num
                                                                        _var70)))
                                                         _var71)))))))))))

(define (subComplex lstcomplex1 lstcomplex2)
        (let ((_var72 (null? lstcomplex1)))
             (if _var72
                 (let ((_var73 '()))
                      _var73)
                 (let ((_var74 (null? lstcomplex2)))
                      (if _var74
                          (let ((_var75 '()))
                               _var75)
                          (let ((cmp1 (car lstcomplex1)))
                               (let ((cmp2 (car lstcomplex2)))
                                    (let ((_var76 (car cmp1)))
                                         (let ((_var78 (car cmp2)))
                                              (let ((_var77 (- _var76
                                                               _var78)))
                                                   (let ((_var81 (cdr cmp1)))
                                                        (let ((_var83 (cdr cmp2)))
                                                             (let ((_var82 (- _var81
                                                                              _var83)))
                                                                  (let ((sub (cons _var77
                                                                                   _var82)))
                                                                       (let ((_var86 (cdr lstcomplex1)))
                                                                            (let ((_var88 (cdr lstcomplex2)))
                                                                                 (let ((_var87 (subComplex _var86
                                                                                                           _var88)))
                                                                                      (let ((_var90 (cons sub
                                                                                                          _var87)))
                                                                                           _var90))))))))))))))))))

(define (plusComplex lstcomplex1 lstcomplex2)
        (let ((_var91 (null? lstcomplex1)))
             (if _var91
                 (let ((_var92 '()))
                      _var92)
                 (let ((cmp1 (car lstcomplex1)))
                      (let ((cmp2 (car lstcomplex2)))
                           (let ((_var93 (car cmp1)))
                                (let ((_var95 (car cmp2)))
                                     (let ((_var94 (+ _var93
                                                      _var95)))
                                          (let ((_var98 (cdr cmp1)))
                                               (let ((_var100 (cdr cmp2)))
                                                    (let ((_var99 (+ _var98
                                                                     _var100)))
                                                         (let ((sub (cons _var94
                                                                          _var99)))
                                                              (let ((_var103 (cdr lstcomplex1)))
                                                                   (let ((_var105 (cdr lstcomplex2)))
                                                                        (let ((_var104 (plusComplex _var103
                                                                                                    _var105)))
                                                                             (let ((_var107 (cons sub
                                                                                                  _var104)))
                                                                                  _var107))))))))))))))))

(define (mulComplex xs ys)
        (let ((_var108 (null? xs)))
             (if _var108
                 (let ((_var109 '()))
                      _var109)
                 (let ((_var110 (car xs)))
                      (let ((x (car _var110)))
                           (let ((_var112 (car xs)))
                                (let ((y (cdr _var112)))
                                     (let ((_var114 (car ys)))
                                          (let ((u (car _var114)))
                                               (let ((_var116 (car ys)))
                                                    (let ((v (cdr _var116)))
                                                         (let ((_var118 (* x
                                                                           u)))
                                                              (let ((_var120 (* y
                                                                                v)))
                                                                   (let ((r (- _var118
                                                                               _var120)))
                                                                        (let ((_var122 (* x
                                                                                          v)))
                                                                             (let ((_var124 (* y
                                                                                               u)))
                                                                                  (let ((i (+ _var122
                                                                                              _var124)))
                                                                                       (let ((_var126 (cons r
                                                                                                            i)))
                                                                                            (let ((_var128 (cdr xs)))
                                                                                                 (let ((_var130 (cdr ys)))
                                                                                                      (let ((_var129 (mulComplex _var128
                                                                                                                                 _var130)))
                                                                                                           (let ((_var127 (cons _var126
                                                                                                                                _var129)))
                                                                                                                _var127))))))))))))))))))))))

(define (rootsOfUnity n)
        (let ((_var133 0))
             (let ((a (thetas n
                              _var133)))
                  (let ((_var135 "cos"))
                       (let ((_var136 (map _var135
                                           a)))
                            (let ((_var138 "sin"))
                                 (let ((_var139 (map _var138
                                                     a)))
                                      (let ((_var137 (makeComplex1 _var136
                                                                   _var139)))
                                           _var137))))))))

(define (thetas n k)
        (let ((_var141 (eq? k
                            n)))
             (if _var141
                 (let ((_var142 '()))
                      _var142)
                 (let ((_var143 6))
                      (let ((_var145 1))
                           (let ((_var146 (* n
                                             _var145)))
                                (let ((_var144 (divide _var143
                                                       _var146)))
                                     (let ((_var149 1))
                                          (let ((_var150 (* k
                                                            _var149)))
                                               (let ((_var148 (* _var144
                                                                 _var150)))
                                                    (let ((_var153 1))
                                                         (let ((_var154 (+ k
                                                                           _var153)))
                                                              (let ((_var155 (thetas n
                                                                                     _var154)))
                                                                   (let ((_var152 (cons _var148
                                                                                        _var155)))
                                                                        _var152))))))))))))))

(define (norm lst1)
        (let ((a (sq_magnitude lst1)))
             (let ((b (sum a)))
                  (let ((_var157 (sqrt b)))
                       _var157))))

(define (sq_magnitude lst)
        (let ((_var158 (null? lst)))
             (if _var158
                 (let ((_var159 '()))
                      _var159)
                 (let ((cmp (car lst)))
                      (let ((_var160 (car cmp)))
                           (let ((_var162 (car cmp)))
                                (let ((_var161 (* _var160
                                                  _var162)))
                                     (let ((_var165 (cdr cmp)))
                                          (let ((_var167 (cdr cmp)))
                                               (let ((_var166 (* _var165
                                                                 _var167)))
                                                    (let ((a (+ _var161
                                                                _var166)))
                                                         (let ((_var170 (cdr lst)))
                                                              (let ((_var171 (sq_magnitude _var170)))
                                                                   (let ((_var172 (cons a
                                                                                        _var171)))
                                                                        _var172))))))))))))))

(define (sum lst)
        (let ((_var173 (null? lst)))
             (if _var173
                 (let ((_var174 0))
                      _var174)
                 (let ((_var175 (car lst)))
                      (let ((_var177 (cdr lst)))
                           (let ((_var178 (sum _var177)))
                                (let ((_var176 (+ _var175
                                                  _var178)))
                                     _var176)))))))

(define (distance lstcomplex1 lstcomplex2)
        (let ((a (subComplex lstcomplex1
                             lstcomplex2)))
             (let ((_var180 (norm a)))
                  _var180)))

(define (fft xs)
        (let ((n (length xs)))
             (let ((_var181 (rootsOfUnity n)))
                  (let ((us (mapConjugate _var181)))
                       (let ((a (ffth xs
                                      us)))
                            (let ((_var183 1))
                                 (let ((_var184 (divide _var183
                                                        n)))
                                      (let ((_var185 (map1 _var184
                                                           a)))
                                           _var185))))))))

(define (fftinv xs)
        (let ((n (length xs)))
             (let ((us (rootsOfUnity n)))
                  (let ((_var186 (ffth xs
                                       us)))
                       _var186))))

(define (ffth xs us)
        (let ((n (length xs)))
             (let ((_var187 1))
                  (let ((_var188 (eq? _var187
                                      n)))
                       (if _var188
                           xs
                           (let ((_var189 1))
                                (let ((_var190 (> n
                                                  _var189)))
                                     (if _var190
                                         (let ((_var191 (odds xs)))
                                              (let ((_var193 (evns us)))
                                                   (let ((fftOdd (ffth _var191
                                                                       _var193)))
                                                        (let ((_var195 (evns xs)))
                                                             (let ((_var197 (evns us)))
                                                                  (let ((fftEvn (ffth _var195
                                                                                      _var197)))
                                                                       (let ((b (times us
                                                                                       fftOdd)))
                                                                            (let ((_var199 (plus b
                                                                                                 fftEvn)))
                                                                                 _var199))))))))
                                         (let ((_var200 11))
                                              (let ((_var202 11))
                                                   (let ((_var201 (cons _var200
                                                                        _var202)))
                                                        _var201)))))))))))

(define (times xs fftOdd)
        (let ((_var204 (length xs)))
             (let ((odd (replikate fftOdd
                                   _var204)))
                  (let ((a (mulComplex xs
                                       odd)))
                       a))))

(define (plus xs fftEvn)
        (let ((_var206 (length xs)))
             (let ((evn (replikate fftEvn
                                   _var206)))
                  (let ((_var208 (plusComplex evn
                                              xs)))
                       _var208))))

(define (evns xs)
        (let ((_var209 2))
             (let ((_var210 (everyNth _var209
                                      xs)))
                  _var210)))

(define (odds xs)
        (let ((_var211 2))
             (let ((_var213 (cdr xs)))
                  (let ((_var212 (everyNth _var211
                                           _var213)))
                       _var212))))

(define (everyNth n xs)
        (let ((ys (takeWhile n
                             xs)))
             (let ((_var215 (mapHead ys)))
                  _var215)))

(define (takeWhile n xs)
        (let ((_var216 (takeWhile2 n
                                   xs)))
             (let ((_var217 (cons xs
                                  _var216)))
                  _var217)))

(define (takeWhile2 n xs)
        (let ((len (length xs)))
             (let ((_var218 (eq? n
                                 len)))
                  (let ((_var220 (< len
                                    n)))
                       (let ((_var219 (or _var218
                                          _var220)))
                            (if _var219
                                (let ((_var222 '()))
                                     _var222)
                                (let ((ys (drop n
                                                xs)))
                                     (let ((_var223 (takeWhile2 n
                                                                ys)))
                                          (let ((_var224 (cons ys
                                                               _var223)))
                                               _var224)))))))))

(define (main n)
        (let ((_var225 0))
             (let ((lstcmplx (makeComplex2 _var225
                                           n)))
                  (let ((_var227 (fft lstcmplx)))
                       (let ((_var228 (fftinv _var227)))
                            (let ((_var229 (distance lstcmplx
                                                     _var228)))
                                 _var229))))))

(define (divide1 a b n op)
        (let ((_var230 (< a
                          b)))
             (if _var230
                 (let ((_var231 0))
                      (let ((_var232 (eq? op
                                          _var231)))
                           (if _var232
                               n
                               a)))
                 (let ((_var233 (- a
                                   b)))
                      (let ((_var235 1))
                           (let ((_var236 (+ _var235
                                             n)))
                                (let ((_var234 (divide1 _var233
                                                        b
                                                        _var236
                                                        op)))
                                     _var234)))))))

(define (divide a b)
        (let ((_var238 0))
             (let ((_var240 0))
                  (let ((_var239 (divide1 a
                                          b
                                          _var238
                                          _var240)))
                       _var239))))

(define (map tag lst)
        (let ((_var242 (null? lst)))
             (if _var242
                 (let ((_var243 '()))
                      _var243)
                 (let ((_var244 "cos"))
                      (let ((_var245 (eq? tag
                                          _var244)))
                           (if _var245
                               (let ((_var246 (car lst)))
                                    (let ((_var247 (cos _var246)))
                                         (let ((_var249 (cdr lst)))
                                              (let ((_var250 (map tag
                                                                  _var249)))
                                                   (let ((_var248 (cons _var247
                                                                        _var250)))
                                                        _var248)))))
                               (let ((_var252 "sin"))
                                    (let ((_var253 (eq? tag
                                                        _var252)))
                                         (if _var253
                                             (let ((_var254 (car lst)))
                                                  (let ((_var255 (sin _var254)))
                                                       (let ((_var257 (cdr lst)))
                                                            (let ((_var258 (map tag
                                                                                _var257)))
                                                                 (let ((_var256 (cons _var255
                                                                                      _var258)))
                                                                      _var256)))))
                                             (let ((_var260 '()))
                                                  _var260))))))))))

(define (mapConjugate cmplx)
        (let ((_var261 (null? cmplx)))
             (if _var261
                 (let ((_var262 '()))
                      _var262)
                 (let ((a (car cmplx)))
                      (let ((_var263 (car a)))
                           (let ((_var265 0))
                                (let ((_var267 1))
                                     (let ((_var266 (- _var265
                                                       _var267)))
                                          (let ((_var270 (cdr a)))
                                               (let ((_var269 (* _var266
                                                                 _var270)))
                                                    (let ((b (cons _var263
                                                                   _var269)))
                                                         (let ((_var273 (cdr cmplx)))
                                                              (let ((_var274 (mapConjugate _var273)))
                                                                   (let ((_var275 (cons b
                                                                                        _var274)))
                                                                        _var275))))))))))))))

(define (map1 n cmplx)
        (let ((_var276 (null? cmplx)))
             (if _var276
                 (let ((_var277 '()))
                      _var277)
                 (let ((a (car cmplx)))
                      (let ((_var278 (car a)))
                           (let ((_var279 (* n
                                             _var278)))
                                (let ((_var281 (cdr a)))
                                     (let ((_var282 (* n
                                                       _var281)))
                                          (let ((b (cons _var279
                                                         _var282)))
                                               (let ((_var284 (cdr cmplx)))
                                                    (let ((_var285 (map1 n
                                                                         _var284)))
                                                         (let ((_var286 (cons b
                                                                              _var285)))
                                                              _var286))))))))))))

(define (mapHead lst)
        (let ((_var287 (null? lst)))
             (if _var287
                 (let ((_var288 '()))
                      _var288)
                 (let ((elem (car lst)))
                      (let ((_var289 (car elem)))
                           (let ((_var291 (cdr lst)))
                                (let ((_var292 (mapHead _var291)))
                                     (let ((_var290 (cons _var289
                                                          _var292)))
                                          _var290))))))))

(define (length lst)
        (let ((_var294 (null? lst)))
             (if _var294
                 (let ((_var295 0))
                      _var295)
                 (let ((_var296 1))
                      (let ((_var298 (cdr lst)))
                           (let ((_var299 (length _var298)))
                                (let ((_var297 (+ _var296
                                                  _var299)))
                                     _var297)))))))

(define (replikate lst n)
        (let ((len (length lst)))
             (let ((_var301 (< len
                               n)))
                  (if _var301
                      (let ((_var302 (cycle lst
                                            lst
                                            n)))
                           _var302)
                      (let ((_var303 (eq? len
                                          n)))
                           (if _var303
                               lst
                               (let ((_var304 (take lst
                                                    n)))
                                    _var304)))))))

(define (cycle lst1 lst2 n)
        (let ((_var305 0))
             (let ((_var306 (eq? _var305
                                 n)))
                  (if _var306
                      (let ((_var307 '()))
                           _var307)
                      (let ((_var308 (null? lst1)))
                           (if _var308
                               (let ((_var309 (cycle lst2
                                                     lst2
                                                     n)))
                                    _var309)
                               (let ((_var310 (car lst1)))
                                    (let ((_var312 (cdr lst1)))
                                         (let ((_var314 1))
                                              (let ((_var315 (- n
                                                                _var314)))
                                                   (let ((_var313 (cycle _var312
                                                                         lst2
                                                                         _var315)))
                                                        (let ((_var311 (cons _var310
                                                                             _var313)))
                                                             _var311))))))))))))

(define (take lst n)
        (let ((_var318 0))
             (let ((_var319 (eq? _var318
                                 n)))
                  (if _var319
                      (let ((_var320 '()))
                           _var320)
                      (let ((_var321 (car lst)))
                           (let ((_var323 (cdr lst)))
                                (let ((_var325 1))
                                     (let ((_var326 (- n
                                                       _var325)))
                                          (let ((_var324 (take _var323
                                                               _var326)))
                                               (let ((_var322 (cons _var321
                                                                    _var324)))
                                                    _var322))))))))))

(define (drop n lst)
        (let ((_var329 0))
             (let ((_var330 (eq? _var329
                                 n)))
                  (if _var330
                      lst
                      (let ((_var331 1))
                           (let ((_var332 (- n
                                             _var331)))
                                (let ((_var334 (cdr lst)))
                                     (let ((_var333 (drop _var332
                                                          _var334)))
                                          _var333))))))))

(let ((_var336 51))
     (let ((_var337 (main _var336)))
          _var337))
