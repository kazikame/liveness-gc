(define (and anx any)
        (let ((_var0 #t))
             (let ((_var1 (eq? anx
                               _var0)))
                  (if _var1
                      any
                      (let ((_var2 #f))
                           _var2)))))

(define (or orx ory)
        (let ((_var3 #t))
             (let ((_var4 (eq? orx
                               _var3)))
                  (if _var4
                      orx
                      ory))))

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

(define (divide1 a b n op)
        (let ((_var11 (< a
                         b)))
             (if _var11
                 (let ((_var12 0))
                      (let ((_var13 (eq? op
                                         _var12)))
                           (if _var13
                               n
                               a)))
                 (let ((_var14 (- a
                                  b)))
                      (let ((_var16 1))
                           (let ((_var17 (+ _var16
                                            n)))
                                (let ((_var15 (divide1 _var14
                                                       b
                                                       _var17
                                                       op)))
                                     _var15)))))))

(define (divide a b)
        (let ((_var19 0))
             (let ((_var21 0))
                  (let ((_var20 (divide1 a
                                         b
                                         _var19
                                         _var21)))
                       _var20))))

(define (take n lst)
        (let ((_var23 0))
             (let ((_var24 (eq? _var23
                                n)))
                  (if _var24
                      (let ((_var25 '()))
                           _var25)
                      (let ((_var26 (car lst)))
                           (let ((_var28 1))
                                (let ((_var29 (- n
                                                 _var28)))
                                     (let ((_var31 (cdr lst)))
                                          (let ((_var30 (take _var29
                                                              _var31)))
                                               (let ((_var27 (cons _var26
                                                                   _var30)))
                                                    _var27))))))))))

(define (drop n lst)
        (let ((_var34 0))
             (let ((_var35 (eq? _var34
                                n)))
                  (if _var35
                      lst
                      (let ((_var36 1))
                           (let ((_var37 (- n
                                            _var36)))
                                (let ((_var39 (cdr lst)))
                                     (let ((_var38 (drop _var37
                                                         _var39)))
                                          _var38))))))))

(define (reverse lst)
        (let ((_var41 (null? lst)))
             (if _var41
                 (let ((_var42 '()))
                      _var42)
                 (let ((_var43 (cdr lst)))
                      (let ((_var44 (reverse _var43)))
                           (let ((_var46 (car lst)))
                                (let ((_var48 '()))
                                     (let ((_var47 (cons _var46
                                                         _var48)))
                                          (let ((_var45 (append _var44
                                                                _var47)))
                                               _var45)))))))))

(define (zip xs ys)
        (let ((_var51 (null? xs)))
             (let ((_var53 (null? ys)))
                  (let ((_var52 (or _var51
                                    _var53)))
                       (if _var52
                           (let ((_var55 '()))
                                _var55)
                           (let ((_var56 (car xs)))
                                (let ((_var58 (car ys)))
                                     (let ((_var57 (cons _var56
                                                         _var58)))
                                          (let ((_var61 (cdr xs)))
                                               (let ((_var63 (cdr ys)))
                                                    (let ((_var62 (zip _var61
                                                                       _var63)))
                                                         (let ((_var60 (cons _var57
                                                                             _var62)))
                                                              _var60))))))))))))

(define (getLength lst)
        (let ((_var66 (null? lst)))
             (if _var66
                 (let ((_var67 0))
                      _var67)
                 (let ((_var68 1))
                      (let ((_var70 (cdr lst)))
                           (let ((_var71 (getLength _var70)))
                                (let ((_var69 (+ _var68
                                                 _var71)))
                                     _var69)))))))

(define (findk k km m pairs)
        (let ((_var73 (null? pairs)))
             (if _var73
                 km
                 (let ((_var74 (car pairs)))
                      (let ((_var75 (car _var74)))
                           (let ((_var77 (car pairs)))
                                (let ((_var78 (cdr _var77)))
                                     (let ((s (+ _var75
                                                 _var78)))
                                          (let ((_var80 (< s
                                                           m)))
                                               (if _var80
                                                   (let ((_var81 1))
                                                        (let ((_var82 (+ _var81
                                                                         k)))
                                                             (let ((_var84 (cdr pairs)))
                                                                  (let ((_var83 (findk _var82
                                                                                       km
                                                                                       m
                                                                                       _var84)))
                                                                       _var83))))
                                                   (let ((_var86 1))
                                                        (let ((_var87 (+ _var86
                                                                         k)))
                                                             (let ((_var89 (cdr pairs)))
                                                                  (let ((_var88 (findk _var87
                                                                                       k
                                                                                       s
                                                                                       _var89)))
                                                                       _var88))))))))))))))

(define (elem n lst)
        (let ((_var91 (null? lst)))
             (if _var91
                 (let ((_var92 0))
                      _var92)
                 (let ((_var93 (car lst)))
                      (let ((_var94 (eq? _var93
                                         n)))
                           (if _var94
                               (let ((_var95 1))
                                    _var95)
                               (let ((_var96 (cdr lst)))
                                    (let ((_var97 (elem n
                                                        _var96)))
                                         _var97))))))))

(define (zip0 lst)
        (let ((_var98 (null? lst)))
             (if _var98
                 (let ((_var99 '()))
                      _var99)
                 (let ((_var100 (car lst)))
                      (let ((_var102 0))
                           (let ((_var101 (cons _var100
                                                _var102)))
                                (let ((_var105 (cdr lst)))
                                     (let ((_var106 (zip0 _var105)))
                                          (let ((_var104 (cons _var101
                                                               _var106)))
                                               _var104)))))))))

(define (mapsnd pairs)
        (let ((_var108 (null? pairs)))
             (if _var108
                 (let ((_var109 '()))
                      _var109)
                 (let ((_var110 (car pairs)))
                      (let ((_var111 (cdr _var110)))
                           (let ((_var113 (cdr pairs)))
                                (let ((_var114 (mapsnd _var113)))
                                     (let ((_var112 (cons _var111
                                                          _var114)))
                                          _var112))))))))

(define (max a b)
        (let ((_var116 (> a
                          b)))
             (if _var116
                 a
                 b)))

(define (algb2 k0j1 k1j1 pairs x)
        (let ((_var117 (null? pairs)))
             (if _var117
                 (let ((_var118 '()))
                      _var118)
                 (let ((_var119 (car pairs)))
                      (let ((k0j (cdr _var119)))
                           (let ((_var121 (car pairs)))
                                (let ((y (car _var121)))
                                     (let ((_var123 (eq? x
                                                         y)))
                                          (if _var123
                                              (let ((_var124 1))
                                                   (let ((kjcurr (+ _var124
                                                                    k0j1)))
                                                        (let ((_var127 (cons y
                                                                             kjcurr)))
                                                             (let ((_var129 (cdr pairs)))
                                                                  (let ((_var130 (algb2 k0j
                                                                                        kjcurr
                                                                                        _var129
                                                                                        x)))
                                                                       (let ((_var128 (cons _var127
                                                                                            _var130)))
                                                                            _var128))))))
                                              (let ((kjcurr (max k1j1
                                                                 k0j)))
                                                   (let ((_var127 (cons y
                                                                        kjcurr)))
                                                        (let ((_var129 (cdr pairs)))
                                                             (let ((_var130 (algb2 k0j
                                                                                   kjcurr
                                                                                   _var129
                                                                                   x)))
                                                                  (let ((_var128 (cons _var127
                                                                                       _var130)))
                                                                       _var128))))))))))))))

(define (algb1 xs pairs)
        (let ((_var132 (null? xs)))
             (if _var132
                 (let ((_var133 (mapsnd pairs)))
                      _var133)
                 (let ((_var134 0))
                      (let ((_var136 0))
                           (let ((_var138 (car xs)))
                                (let ((n (algb2 _var134
                                                _var136
                                                pairs
                                                _var138)))
                                     (let ((_var140 (cdr xs)))
                                          (let ((_var141 (algb1 _var140
                                                                n)))
                                               _var141)))))))))

(define (algb xs ys)
        (let ((_var142 0))
             (let ((_var144 (zip0 ys)))
                  (let ((_var145 (algb1 xs
                                        _var144)))
                       (let ((_var143 (cons _var142
                                            _var145)))
                            _var143)))))

(define (algc m n xs ys lst)
        (let ((_var147 (null? ys)))
             (if _var147
                 lst
                 (let ((_var148 1))
                      (let ((_var150 (getLength xs)))
                           (let ((_var149 (eq? _var148
                                               _var150)))
                                (if _var149
                                    (let ((_var152 1))
                                         (let ((_var154 (car xs)))
                                              (let ((_var155 (elem _var154
                                                                   ys)))
                                                   (let ((_var153 (eq? _var152
                                                                       _var155)))
                                                        (if _var153
                                                            (let ((_var157 (car xs)))
                                                                 (let ((_var158 (cons _var157
                                                                                      lst)))
                                                                      _var158))
                                                            lst)))))
                                    (let ((_var159 2))
                                         (let ((m2 (divide m
                                                           _var159)))
                                              (let ((xs1 (take m2
                                                               xs)))
                                                   (let ((xs2 (drop m2
                                                                    xs)))
                                                        (let ((l1 (algb xs1
                                                                        ys)))
                                                             (let ((_var161 (reverse xs2)))
                                                                  (let ((_var163 (reverse ys)))
                                                                       (let ((_var162 (algb _var161
                                                                                            _var163)))
                                                                            (let ((l2 (reverse _var162)))
                                                                                 (let ((_var166 0))
                                                                                      (let ((_var168 0))
                                                                                           (let ((_var170 0))
                                                                                                (let ((_var172 1))
                                                                                                     (let ((_var171 (- _var170
                                                                                                                       _var172)))
                                                                                                          (let ((_var175 (zip l1
                                                                                                                              l2)))
                                                                                                               (let ((k (findk _var166
                                                                                                                               _var168
                                                                                                                               _var171
                                                                                                                               _var175)))
                                                                                                                    (let ((_var177 (- m
                                                                                                                                      m2)))
                                                                                                                         (let ((_var179 (- n
                                                                                                                                           k)))
                                                                                                                              (let ((_var181 (drop k
                                                                                                                                                   ys)))
                                                                                                                                   (let ((ex1 (algc _var177
                                                                                                                                                    _var179
                                                                                                                                                    xs2
                                                                                                                                                    _var181
                                                                                                                                                    lst)))
                                                                                                                                        (let ((_var183 (take k
                                                                                                                                                             ys)))
                                                                                                                                             (let ((_var184 (algc m2
                                                                                                                                                                  k
                                                                                                                                                                  xs1
                                                                                                                                                                  _var183
                                                                                                                                                                  ex1)))
                                                                                                                                                  _var184)))))))))))))))))))))))))))))

(define (lcss xs ys)
        (let ((_var185 (getLength xs)))
             (let ((_var187 (getLength ys)))
                  (let ((_var189 '()))
                       (let ((_var186 (algc _var185
                                            _var187
                                            xs
                                            ys
                                            _var189)))
                            _var186)))))

(define (makeList n)
        (let ((_var191 0))
             (let ((_var192 (eq? _var191
                                 n)))
                  (if _var192
                      (let ((_var193 '()))
                           _var193)
                      (let ((_var194 1))
                           (let ((_var195 (- n
                                             _var194)))
                                (let ((_var196 (makeList _var195)))
                                     (let ((_var197 (cons n
                                                          _var196)))
                                          _var197))))))))

(let ((_var198 10))
     (let ((a (makeList _var198)))
          (let ((_var200 5))
               (let ((b (makeList _var200)))
                    (let ((_var202 (lcss a
                                         b)))
                         _var202)))))