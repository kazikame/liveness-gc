(define (or a b)
        (if a
            a
            b))

(define (append ax ay)
        (let ((_var0 (null? ax)))
             (if _var0
                 ay
                 (let ((_var1 (car ax)))
                      (let ((_var3 (cdr ax)))
                           (let ((_var4 (append _var3
                                                ay)))
                                (let ((_var2 (cons _var1
                                                   _var4)))
                                     _var2)))))))

(define (divide1 a b n op)
        (let ((_var6 (< a
                        b)))
             (if _var6
                 (let ((_var7 0))
                      (let ((_var8 (eq? op
                                        _var7)))
                           (if _var8
                               n
                               a)))
                 (let ((_var9 (- a
                                 b)))
                      (let ((_var11 1))
                           (let ((_var12 (+ _var11
                                            n)))
                                (let ((_var10 (divide1 _var9
                                                       b
                                                       _var12
                                                       op)))
                                     _var10)))))))

(define (divide a b)
        (let ((_var14 0))
             (let ((_var16 0))
                  (let ((_var15 (divide1 a
                                         b
                                         _var14
                                         _var16)))
                       _var15))))

(define (take n lst)
        (let ((_var18 0))
             (let ((_var19 (eq? _var18
                                n)))
                  (if _var19
                      (let ((_var20 '()))
                           _var20)
                      (let ((_var21 (car lst)))
                           (let ((_var23 1))
                                (let ((_var24 (- n
                                                 _var23)))
                                     (let ((_var26 (cdr lst)))
                                          (let ((_var25 (take _var24
                                                              _var26)))
                                               (let ((_var22 (cons _var21
                                                                   _var25)))
                                                    _var22))))))))))

(define (drop n lst)
        (let ((_var29 0))
             (let ((_var30 (eq? _var29
                                n)))
                  (if _var30
                      lst
                      (let ((_var31 1))
                           (let ((_var32 (- n
                                            _var31)))
                                (let ((_var34 (cdr lst)))
                                     (let ((_var33 (drop _var32
                                                         _var34)))
                                          _var33))))))))

(define (reverse lst)
        (let ((_var36 (null? lst)))
             (if _var36
                 (let ((_var37 '()))
                      _var37)
                 (let ((_var38 (cdr lst)))
                      (let ((_var39 (reverse _var38)))
                           (let ((_var41 (car lst)))
                                (let ((_var43 '()))
                                     (let ((_var42 (cons _var41
                                                         _var43)))
                                          (let ((_var40 (append _var39
                                                                _var42)))
                                               _var40)))))))))

(define (zip xs ys)
        (let ((_var46 (null? xs)))
             (let ((_var48 (null? ys)))
                  (let ((_var47 (or _var46
                                    _var48)))
                       (if _var47
                           (let ((_var50 '()))
                                _var50)
                           (let ((_var51 (car xs)))
                                (let ((_var53 (car ys)))
                                     (let ((_var52 (cons _var51
                                                         _var53)))
                                          (let ((_var56 (cdr xs)))
                                               (let ((_var58 (cdr ys)))
                                                    (let ((_var57 (zip _var56
                                                                       _var58)))
                                                         (let ((_var55 (cons _var52
                                                                             _var57)))
                                                              _var55))))))))))))

(define (getLength lst)
        (let ((_var61 (null? lst)))
             (if _var61
                 (let ((_var62 0))
                      _var62)
                 (let ((_var63 1))
                      (let ((_var65 (cdr lst)))
                           (let ((_var66 (getLength _var65)))
                                (let ((_var64 (+ _var63
                                                 _var66)))
                                     _var64)))))))

(define (findk k km m pairs)
        (let ((_var68 (null? pairs)))
             (if _var68
                 km
                 (let ((_var69 (car pairs)))
                      (let ((_var70 (car _var69)))
                           (let ((_var72 (car pairs)))
                                (let ((_var73 (cdr _var72)))
                                     (let ((s (+ _var70
                                                 _var73)))
                                          (let ((_var75 (< s
                                                           m)))
                                               (if _var75
                                                   (let ((_var76 1))
                                                        (let ((_var77 (+ _var76
                                                                         k)))
                                                             (let ((_var79 (cdr pairs)))
                                                                  (let ((_var78 (findk _var77
                                                                                       km
                                                                                       m
                                                                                       _var79)))
                                                                       _var78))))
                                                   (let ((_var81 1))
                                                        (let ((_var82 (+ _var81
                                                                         k)))
                                                             (let ((_var84 (cdr pairs)))
                                                                  (let ((_var83 (findk _var82
                                                                                       k
                                                                                       s
                                                                                       _var84)))
                                                                       _var83))))))))))))))

(define (elem n lst)
        (let ((_var86 (null? lst)))
             (if _var86
                 (let ((_var87 0))
                      _var87)
                 (let ((_var88 (car lst)))
                      (let ((_var89 (eq? _var88
                                         n)))
                           (if _var89
                               (let ((_var90 1))
                                    _var90)
                               (let ((_var91 (cdr lst)))
                                    (let ((_var92 (elem n
                                                        _var91)))
                                         _var92))))))))

(define (zip0 lst)
        (let ((_var93 (null? lst)))
             (if _var93
                 (let ((_var94 '()))
                      _var94)
                 (let ((_var95 (car lst)))
                      (let ((_var97 0))
                           (let ((_var96 (cons _var95
                                               _var97)))
                                (let ((_var100 (cdr lst)))
                                     (let ((_var101 (zip0 _var100)))
                                          (let ((_var99 (cons _var96
                                                              _var101)))
                                               _var99)))))))))

(define (mapsnd pairs)
        (let ((_var103 (null? pairs)))
             (if _var103
                 (let ((_var104 '()))
                      _var104)
                 (let ((_var105 (car pairs)))
                      (let ((_var106 (cdr _var105)))
                           (let ((_var108 (cdr pairs)))
                                (let ((_var109 (mapsnd _var108)))
                                     (let ((_var107 (cons _var106
                                                          _var109)))
                                          _var107))))))))

(define (max a b)
        (let ((_var111 (> a
                          b)))
             (if _var111
                 a
                 b)))

(define (algb2 k0j1 k1j1 pairs x)
        (let ((_var112 (null? pairs)))
             (if _var112
                 (let ((_var113 '()))
                      _var113)
                 (let ((_var114 (car pairs)))
                      (let ((k0j (cdr _var114)))
                           (let ((_var116 (car pairs)))
                                (let ((y (car _var116)))
                                     (let ((_var118 (eq? x
                                                         y)))
                                          (if _var118
                                              (let ((_var119 1))
                                                   (let ((kjcurr (+ _var119
                                                                    k0j1)))
                                                        (let ((_var122 (cons y
                                                                             kjcurr)))
                                                             (let ((_var124 (cdr pairs)))
                                                                  (let ((_var125 (algb2 k0j
                                                                                        kjcurr
                                                                                        _var124
                                                                                        x)))
                                                                       (let ((_var123 (cons _var122
                                                                                            _var125)))
                                                                            _var123))))))
                                              (let ((kjcurr (max k1j1
                                                                 k0j)))
                                                   (let ((_var122 (cons y
                                                                        kjcurr)))
                                                        (let ((_var124 (cdr pairs)))
                                                             (let ((_var125 (algb2 k0j
                                                                                   kjcurr
                                                                                   _var124
                                                                                   x)))
                                                                  (let ((_var123 (cons _var122
                                                                                       _var125)))
                                                                       _var123))))))))))))))

(define (algb1 xs pairs)
        (let ((_var127 (null? xs)))
             (if _var127
                 (let ((_var128 (mapsnd pairs)))
                      _var128)
                 (let ((_var129 0))
                      (let ((_var131 0))
                           (let ((_var133 (car xs)))
                                (let ((n (algb2 _var129
                                                _var131
                                                pairs
                                                _var133)))
                                     (let ((_var135 (cdr xs)))
                                          (let ((_var136 (algb1 _var135
                                                                n)))
                                               _var136)))))))))

(define (algb xs ys)
        (let ((_var137 0))
             (let ((_var139 (zip0 ys)))
                  (let ((_var140 (algb1 xs
                                        _var139)))
                       (let ((_var138 (cons _var137
                                            _var140)))
                            _var138)))))

(define (algc m n xs ys lst)
        (let ((_var142 (null? ys)))
             (if _var142
                 lst
                 (let ((_var143 1))
                      (let ((_var145 (getLength xs)))
                           (let ((_var144 (eq? _var143
                                               _var145)))
                                (if _var144
                                    (let ((_var147 1))
                                         (let ((_var149 (car xs)))
                                              (let ((_var150 (elem _var149
                                                                   ys)))
                                                   (let ((_var148 (eq? _var147
                                                                       _var150)))
                                                        (if _var148
                                                            (let ((_var152 (car xs)))
                                                                 (let ((_var153 (cons _var152
                                                                                      lst)))
                                                                      _var153))
                                                            lst)))))
                                    (let ((_var154 2))
                                         (let ((m2 (divide m
                                                           _var154)))
                                              (let ((xs1 (take m2
                                                               xs)))
                                                   (let ((xs2 (drop m2
                                                                    xs)))
                                                        (let ((l1 (algb xs1
                                                                        ys)))
                                                             (let ((_var156 (reverse xs2)))
                                                                  (let ((_var158 (reverse ys)))
                                                                       (let ((_var157 (algb _var156
                                                                                            _var158)))
                                                                            (let ((l2 (reverse _var157)))
                                                                                 (let ((_var161 0))
                                                                                      (let ((_var163 0))
                                                                                           (let ((_var165 0))
                                                                                                (let ((_var167 1))
                                                                                                     (let ((_var166 (- _var165
                                                                                                                       _var167)))
                                                                                                          (let ((_var170 (zip l1
                                                                                                                              l2)))
                                                                                                               (let ((k (findk _var161
                                                                                                                               _var163
                                                                                                                               _var166
                                                                                                                               _var170)))
                                                                                                                    (let ((_var172 (- m
                                                                                                                                      m2)))
                                                                                                                         (let ((_var174 (- n
                                                                                                                                           k)))
                                                                                                                              (let ((_var176 (drop k
                                                                                                                                                   ys)))
                                                                                                                                   (let ((ex1 (algc _var172
                                                                                                                                                    _var174
                                                                                                                                                    xs2
                                                                                                                                                    _var176
                                                                                                                                                    lst)))
                                                                                                                                        (let ((_var178 (take k
                                                                                                                                                             ys)))
                                                                                                                                             (let ((_var179 (algc m2
                                                                                                                                                                  k
                                                                                                                                                                  xs1
                                                                                                                                                                  _var178
                                                                                                                                                                  ex1)))
                                                                                                                                                  _var179)))))))))))))))))))))))))))))

(define (lcss xs ys)
        (let ((_var180 (getLength xs)))
             (let ((_var182 (getLength ys)))
                  (let ((_var184 '()))
                       (let ((_var181 (algc _var180
                                            _var182
                                            xs
                                            ys
                                            _var184)))
                            _var181)))))

(define (makeList n)
        (let ((_var186 0))
             (let ((_var187 (eq? _var186
                                 n)))
                  (if _var187
                      (let ((_var188 '()))
                           _var188)
                      (let ((_var189 1))
                           (let ((_var190 (- n
                                             _var189)))
                                (let ((_var191 (makeList _var190)))
                                     (let ((_var192 (cons n
                                                          _var191)))
                                          _var192))))))))

(define (run-in-loop a b n)
        (let ((_var193 0))
             (let ((_var194 (= n
                               _var193)))
                  (if _var194
                      (let ((_var195 '()))
                           _var195)
                      (let ((_var196 (lcss a
                                           b)))
                           (let ((_var198 1))
                                (let ((_var199 (- n
                                                  _var198)))
                                     (let ((_var200 (run-in-loop a
                                                                 b
                                                                 _var199)))
                                          (let ((_var197 (cons _var196
                                                               _var200)))
                                               _var197)))))))))

(let ((_var202 100))
     (let ((a (makeList _var202)))
          (let ((_var204 90))
               (let ((b (makeList _var204)))
                    (let ((_var206 8))
                         (let ((_var207 (run-in-loop a
                                                     b
                                                     _var206)))
                              _var207))))))
