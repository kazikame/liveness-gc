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
                                 (let ((_var11 (/ sqrx
                                                  _var10)))
                                      (let ((_var13 4))
                                           (let ((_var14 (factorial _var13)))
                                                (let ((_var15 (/ fourx
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
                                 (let ((_var20 (/ cubex
                                                  _var19)))
                                      (let ((_var22 5))
                                           (let ((_var23 (factorial _var22)))
                                                (let ((_var24 (/ fivex
                                                                 _var23)))
                                                     (let ((_var21 (- _var20
                                                                      _var24)))
                                                          (let ((_var26 (- x
                                                                           _var21)))
                                                               _var26))))))))))))

(define (good-enough? x y)
        (let ((_var27 1))
             (let ((_var29 100))
                  (let ((_var28 (/ _var27
                                   _var29)))
                       (let ((_var32 (- x
                                        y)))
                            (let ((_var33 (abs _var32)))
                                 (let ((_var31 (> _var28
                                                  _var33)))
                                      _var31)))))))

(define (better-guess a b)
        (let ((_var35 (+ a
                         b)))
             (let ((_var37 2))
                  (let ((_var36 (/ _var35
                                   _var37)))
                       _var36))))

(define (sqrt-prime guess x)
        (let ((_var39 (good-enough? guess
                                    x)))
             (if _var39
                 guess
                 (let ((_var40 (better-guess guess
                                             x)))
                      (let ((_var41 (sqrt-prime _var40
                                                x)))
                           _var41)))))

(define (abs x)
        (let ((_var42 0))
             (let ((_var43 (> x
                              _var42)))
                  (if _var43
                      x
                      (let ((_var44 0))
                           (let ((_var45 (- _var44
                                            x)))
                                _var45))))))

(define (sqrt x)
        (let ((_var46 1))
             (let ((_var47 (sqrt-prime _var46
                                       x)))
                  _var47)))

(define (makeComplex1 lst1 lst2)
        (let ((_var48 (null? lst1)))
             (if _var48
                 lst1
                 (let ((_var49 (car lst1)))
                      (let ((_var51 (car lst2)))
                           (let ((_var50 (cons _var49
                                               _var51)))
                                (let ((_var54 (cdr lst1)))
                                     (let ((_var56 (cdr lst2)))
                                          (let ((_var55 (makeComplex1 _var54
                                                                      _var56)))
                                               (let ((_var53 (cons _var50
                                                                   _var55)))
                                                    _var53))))))))))

(define (makeComplex2 i n)
        (let ((_var59 (eq? i
                           n)))
             (if _var59
                 (let ((_var60 '()))
                      _var60)
                 (let ((_var61 1))
                      (let ((_var62 (* _var61
                                       i)))
                           (let ((_var64 0))
                                (let ((num (cons _var62
                                                 _var64)))
                                     (let ((_var66 1))
                                          (let ((_var67 (+ i
                                                           _var66)))
                                               (let ((_var68 (makeComplex2 _var67
                                                                           n)))
                                                    (let ((_var69 (cons num
                                                                        _var68)))
                                                         _var69)))))))))))

(define (subComplex lstcomplex1 lstcomplex2)
        (let ((_var70 (null? lstcomplex1)))
             (if _var70
                 (let ((_var71 '()))
                      _var71)
                 (let ((_var72 (null? lstcomplex2)))
                      (if _var72
                          (let ((_var73 '()))
                               _var73)
                          (let ((cmp1 (car lstcomplex1)))
                               (let ((cmp2 (car lstcomplex2)))
                                    (let ((_var74 (car cmp1)))
                                         (let ((_var76 (car cmp2)))
                                              (let ((_var75 (- _var74
                                                               _var76)))
                                                   (let ((_var79 (cdr cmp1)))
                                                        (let ((_var81 (cdr cmp2)))
                                                             (let ((_var80 (- _var79
                                                                              _var81)))
                                                                  (let ((sub (cons _var75
                                                                                   _var80)))
                                                                       (let ((_var84 (cdr lstcomplex1)))
                                                                            (let ((_var86 (cdr lstcomplex2)))
                                                                                 (let ((_var85 (subComplex _var84
                                                                                                           _var86)))
                                                                                      (let ((_var88 (cons sub
                                                                                                          _var85)))
                                                                                           _var88))))))))))))))))))

(define (plusComplex lstcomplex1 lstcomplex2)
        (let ((_var89 (null? lstcomplex1)))
             (if _var89
                 (let ((_var90 '()))
                      _var90)
                 (let ((cmp1 (car lstcomplex1)))
                      (let ((cmp2 (car lstcomplex2)))
                           (let ((_var91 (car cmp1)))
                                (let ((_var93 (car cmp2)))
                                     (let ((_var92 (+ _var91
                                                      _var93)))
                                          (let ((_var96 (cdr cmp1)))
                                               (let ((_var98 (cdr cmp2)))
                                                    (let ((_var97 (+ _var96
                                                                     _var98)))
                                                         (let ((sub (cons _var92
                                                                          _var97)))
                                                              (let ((_var101 (cdr lstcomplex1)))
                                                                   (let ((_var103 (cdr lstcomplex2)))
                                                                        (let ((_var102 (plusComplex _var101
                                                                                                    _var103)))
                                                                             (let ((_var105 (cons sub
                                                                                                  _var102)))
                                                                                  _var105))))))))))))))))

(define (mulComplex xs ys)
        (let ((_var106 (null? xs)))
             (if _var106
                 (let ((_var107 '()))
                      _var107)
                 (let ((_var108 (car xs)))
                      (let ((x (car _var108)))
                           (let ((_var110 (car xs)))
                                (let ((y (cdr _var110)))
                                     (let ((_var112 (car ys)))
                                          (let ((u (car _var112)))
                                               (let ((_var114 (car ys)))
                                                    (let ((v (cdr _var114)))
                                                         (let ((_var116 (* x
                                                                           u)))
                                                              (let ((_var118 (* y
                                                                                v)))
                                                                   (let ((r (- _var116
                                                                               _var118)))
                                                                        (let ((_var120 (* x
                                                                                          v)))
                                                                             (let ((_var122 (* y
                                                                                               u)))
                                                                                  (let ((i (+ _var120
                                                                                              _var122)))
                                                                                       (let ((_var124 (cons r
                                                                                                            i)))
                                                                                            (let ((_var126 (cdr xs)))
                                                                                                 (let ((_var128 (cdr ys)))
                                                                                                      (let ((_var127 (mulComplex _var126
                                                                                                                                 _var128)))
                                                                                                           (let ((_var125 (cons _var124
                                                                                                                                _var127)))
                                                                                                                _var125))))))))))))))))))))))

(define (rootsOfUnity n)
        (let ((_var131 0))
             (let ((a (thetas n
                              _var131)))
                  (let ((_var133 "cos"))
                       (let ((_var134 (map _var133
                                           a)))
                            (let ((_var136 "sin"))
                                 (let ((_var137 (map _var136
                                                     a)))
                                      (let ((_var135 (makeComplex1 _var134
                                                                   _var137)))
                                           _var135))))))))

(define (thetas n k)
        (let ((_var139 (eq? k
                            n)))
             (if _var139
                 (let ((_var140 '()))
                      _var140)
                 (let ((_var141 2))
                      (let ((_var143 3))
                           (let ((_var142 (* _var141
                                             _var143)))
                                (let ((_var146 1))
                                     (let ((_var147 (* n
                                                       _var146)))
                                          (let ((_var145 (/ _var142
                                                            _var147)))
                                               (let ((_var150 1))
                                                    (let ((_var151 (* k
                                                                      _var150)))
                                                         (let ((_var149 (* _var145
                                                                           _var151)))
                                                              (let ((_var154 1))
                                                                   (let ((_var155 (+ k
                                                                                     _var154)))
                                                                        (let ((_var156 (thetas n
                                                                                               _var155)))
                                                                             (let ((_var153 (cons _var149
                                                                                                  _var156)))
                                                                                  _var153))))))))))))))))

(define (norm lst1)
        (let ((a (sq_magnitude lst1)))
             (let ((b (sum a)))
                  (let ((_var158 (sqrt b)))
                       _var158))))

(define (sq_magnitude lst)
        (let ((_var159 (null? lst)))
             (if _var159
                 (let ((_var160 '()))
                      _var160)
                 (let ((cmp (car lst)))
                      (let ((_var161 (car cmp)))
                           (let ((_var163 (car cmp)))
                                (let ((_var162 (* _var161
                                                  _var163)))
                                     (let ((_var166 (cdr cmp)))
                                          (let ((_var168 (cdr cmp)))
                                               (let ((_var167 (* _var166
                                                                 _var168)))
                                                    (let ((a (+ _var162
                                                                _var167)))
                                                         (let ((_var171 (cdr lst)))
                                                              (let ((_var172 (sq_magnitude _var171)))
                                                                   (let ((_var173 (cons a
                                                                                        _var172)))
                                                                        _var173))))))))))))))

(define (sum lst)
        (let ((_var174 (null? lst)))
             (if _var174
                 (let ((_var175 0))
                      _var175)
                 (let ((_var176 (car lst)))
                      (let ((_var178 (cdr lst)))
                           (let ((_var179 (sum _var178)))
                                (let ((_var177 (+ _var176
                                                  _var179)))
                                     _var177)))))))

(define (distance lstcomplex1 lstcomplex2)
        (let ((a (subComplex lstcomplex1
                             lstcomplex2)))
             (let ((_var181 (norm a)))
                  _var181)))

(define (fft xs)
        (let ((n (length xs)))
             (let ((_var182 (rootsOfUnity n)))
                  (let ((us (mapConjugate _var182)))
                       (let ((a (ffth xs
                                      us)))
                            (let ((_var184 1))
                                 (let ((_var185 (/ _var184
                                                   n)))
                                      (let ((_var186 (map1 _var185
                                                           a)))
                                           _var186))))))))

(define (fftinv xs)
        (let ((n (length xs)))
             (let ((us (rootsOfUnity n)))
                  (let ((_var187 (ffth xs
                                       us)))
                       _var187))))

(define (ffth xs us)
        (let ((n (length xs)))
             (let ((_var188 1))
                  (let ((_var189 (eq? _var188
                                      n)))
                       (if _var189
                           xs
                           (let ((_var190 1))
                                (let ((_var191 (> n
                                                  _var190)))
                                     (if _var191
                                         (let ((_var192 (odds xs)))
                                              (let ((_var194 (evns us)))
                                                   (let ((fftOdd (ffth _var192
                                                                       _var194)))
                                                        (let ((_var196 (evns xs)))
                                                             (let ((_var198 (evns us)))
                                                                  (let ((fftEvn (ffth _var196
                                                                                      _var198)))
                                                                       (let ((b (times us
                                                                                       fftOdd)))
                                                                            (let ((_var200 (plus b
                                                                                                 fftEvn)))
                                                                                 _var200))))))))
                                         (let ((_var201 11))
                                              (let ((_var203 11))
                                                   (let ((_var202 (cons _var201
                                                                        _var203)))
                                                        _var202)))))))))))

(define (times xs fftOdd)
        (let ((_var205 (length xs)))
             (let ((odd (replikate fftOdd
                                   _var205)))
                  (let ((a (mulComplex xs
                                       odd)))
                       a))))

(define (plus xs fftEvn)
        (let ((_var207 (length xs)))
             (let ((evn (replikate fftEvn
                                   _var207)))
                  (let ((_var209 (plusComplex evn
                                              xs)))
                       _var209))))

(define (evns xs)
        (let ((_var210 2))
             (let ((_var211 (everyNth _var210
                                      xs)))
                  _var211)))

(define (odds xs)
        (let ((_var212 2))
             (let ((_var214 (cdr xs)))
                  (let ((_var213 (everyNth _var212
                                           _var214)))
                       _var213))))

(define (everyNth n xs)
        (let ((ys (takeWhile n
                             xs)))
             (let ((_var216 (mapHead ys)))
                  _var216)))

(define (takeWhile n xs)
        (let ((_var217 (takeWhile2 n
                                   xs)))
             (let ((_var218 (cons xs
                                  _var217)))
                  _var218)))

(define (takeWhile2 n xs)
        (let ((len (length xs)))
             (let ((_var219 (eq? n
                                 len)))
                  (let ((_var221 (< len
                                    n)))
                       (let ((_var220 (or _var219
                                          _var221)))
                            (if _var220
                                (let ((_var223 '()))
                                     _var223)
                                (let ((ys (drop n
                                                xs)))
                                     (let ((_var224 (takeWhile2 n
                                                                ys)))
                                          (let ((_var225 (cons ys
                                                               _var224)))
                                               _var225)))))))))

(define (main n)
        (let ((_var226 0))
             (let ((lstcmplx (makeComplex2 _var226
                                           n)))
                  (let ((_var228 (fft lstcmplx)))
                       (let ((_var229 (fftinv _var228)))
                            (let ((_var230 (distance lstcmplx
                                                     _var229)))
                                 _var230))))))

(define (divide1 a b n op)
        (let ((_var231 (< a
                          b)))
             (if _var231
                 (let ((_var232 0))
                      (let ((_var233 (eq? op
                                          _var232)))
                           (if _var233
                               n
                               a)))
                 (let ((_var234 (- a
                                   b)))
                      (let ((_var236 1))
                           (let ((_var237 (+ _var236
                                             n)))
                                (let ((_var235 (divide1 _var234
                                                        b
                                                        _var237
                                                        op)))
                                     _var235)))))))

(define (divide a b)
        (let ((_var239 0))
             (let ((_var241 0))
                  (let ((_var240 (divide1 a
                                          b
                                          _var239
                                          _var241)))
                       _var240))))

(define (map tag lst)
        (let ((_var243 (null? lst)))
             (if _var243
                 (let ((_var244 '()))
                      _var244)
                 (let ((_var245 "cos"))
                      (let ((_var246 (eq? tag
                                          _var245)))
                           (if _var246
                               (let ((_var247 (car lst)))
                                    (let ((_var248 (cos _var247)))
                                         (let ((_var250 (cdr lst)))
                                              (let ((_var251 (map tag
                                                                  _var250)))
                                                   (let ((_var249 (cons _var248
                                                                        _var251)))
                                                        _var249)))))
                               (let ((_var253 "sin"))
                                    (let ((_var254 (eq? tag
                                                        _var253)))
                                         (if _var254
                                             (let ((_var255 (car lst)))
                                                  (let ((_var256 (sin _var255)))
                                                       (let ((_var258 (cdr lst)))
                                                            (let ((_var259 (map tag
                                                                                _var258)))
                                                                 (let ((_var257 (cons _var256
                                                                                      _var259)))
                                                                      _var257)))))
                                             (let ((_var261 '()))
                                                  _var261))))))))))

(define (mapConjugate cmplx)
        (let ((_var262 (null? cmplx)))
             (if _var262
                 (let ((_var263 '()))
                      _var263)
                 (let ((a (car cmplx)))
                      (let ((_var264 (car a)))
                           (let ((_var266 0))
                                (let ((_var268 1))
                                     (let ((_var267 (- _var266
                                                       _var268)))
                                          (let ((_var271 (cdr a)))
                                               (let ((_var270 (* _var267
                                                                 _var271)))
                                                    (let ((b (cons _var264
                                                                   _var270)))
                                                         (let ((_var274 (cdr cmplx)))
                                                              (let ((_var275 (mapConjugate _var274)))
                                                                   (let ((_var276 (cons b
                                                                                        _var275)))
                                                                        _var276))))))))))))))

(define (map1 n cmplx)
        (let ((_var277 (null? cmplx)))
             (if _var277
                 (let ((_var278 '()))
                      _var278)
                 (let ((a (car cmplx)))
                      (let ((_var279 (car a)))
                           (let ((_var280 (* n
                                             _var279)))
                                (let ((_var282 (cdr a)))
                                     (let ((_var283 (* n
                                                       _var282)))
                                          (let ((b (cons _var280
                                                         _var283)))
                                               (let ((_var285 (cdr cmplx)))
                                                    (let ((_var286 (map1 n
                                                                         _var285)))
                                                         (let ((_var287 (cons b
                                                                              _var286)))
                                                              _var287))))))))))))

(define (mapHead lst)
        (let ((_var288 (null? lst)))
             (if _var288
                 (let ((_var289 '()))
                      _var289)
                 (let ((elem (car lst)))
                      (let ((_var290 (car elem)))
                           (let ((_var292 (cdr lst)))
                                (let ((_var293 (mapHead _var292)))
                                     (let ((_var291 (cons _var290
                                                          _var293)))
                                          _var291))))))))

(define (length lst)
        (let ((_var295 (null? lst)))
             (if _var295
                 (let ((_var296 0))
                      _var296)
                 (let ((_var297 1))
                      (let ((_var299 (cdr lst)))
                           (let ((_var300 (length _var299)))
                                (let ((_var298 (+ _var297
                                                  _var300)))
                                     _var298)))))))

(define (replikate lst n)
        (let ((len (length lst)))
             (let ((_var302 (< len
                               n)))
                  (if _var302
                      (let ((_var303 (cycle lst
                                            lst
                                            n)))
                           _var303)
                      (let ((_var304 (eq? len
                                          n)))
                           (if _var304
                               lst
                               (let ((_var305 (take lst
                                                    n)))
                                    _var305)))))))

(define (cycle lst1 lst2 n)
        (let ((_var306 0))
             (let ((_var307 (eq? _var306
                                 n)))
                  (if _var307
                      (let ((_var308 '()))
                           _var308)
                      (let ((_var309 (null? lst1)))
                           (if _var309
                               (let ((_var310 (cycle lst2
                                                     lst2
                                                     n)))
                                    _var310)
                               (let ((_var311 (car lst1)))
                                    (let ((_var313 (cdr lst1)))
                                         (let ((_var315 1))
                                              (let ((_var316 (- n
                                                                _var315)))
                                                   (let ((_var314 (cycle _var313
                                                                         lst2
                                                                         _var316)))
                                                        (let ((_var312 (cons _var311
                                                                             _var314)))
                                                             _var312))))))))))))

(define (take lst n)
        (let ((_var319 0))
             (let ((_var320 (eq? _var319
                                 n)))
                  (if _var320
                      (let ((_var321 '()))
                           _var321)
                      (let ((_var322 (car lst)))
                           (let ((_var324 (cdr lst)))
                                (let ((_var326 1))
                                     (let ((_var327 (- n
                                                       _var326)))
                                          (let ((_var325 (take _var324
                                                               _var327)))
                                               (let ((_var323 (cons _var322
                                                                    _var325)))
                                                    _var323))))))))))

(define (drop n lst)
        (let ((_var330 0))
             (let ((_var331 (eq? _var330
                                 n)))
                  (if _var331
                      lst
                      (let ((_var332 1))
                           (let ((_var333 (- n
                                             _var332)))
                                (let ((_var335 (cdr lst)))
                                     (let ((_var334 (drop _var333
                                                          _var335)))
                                          _var334))))))))

(let ((_var337 10))
     (let ((_var338 (main _var337)))
          _var338))