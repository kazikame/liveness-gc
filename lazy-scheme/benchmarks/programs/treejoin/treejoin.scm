(define (or orx ory)
        (let ((_var0 #t))
             (let ((_var1 (eq? orx
                               _var0)))
                  (if _var1
                      orx
                      ory))))

(define (makeNode k t1 t2)
        (let ((_var2 "node"))
             (let ((_var3 (cons _var2
                                k)))
                  (let ((_var5 '()))
                       (let ((_var6 (cons t2
                                          _var5)))
                            (let ((_var7 (cons t1
                                               _var6)))
                                 (let ((_var4 (cons _var3
                                                    _var7)))
                                      _var4)))))))

(define (makeLeaf k e)
        (let ((_var9 "leaf"))
             (let ((_var10 (cons _var9
                                 k)))
                  (let ((_var12 '()))
                       (let ((_var13 (cons e
                                           _var12)))
                            (let ((_var11 (cons _var10
                                                _var13)))
                                 _var11))))))

(define (insertT k e t)
        (let ((_var15 (car t)))
             (let ((_var17 "empty"))
                  (let ((_var16 (eq? _var15
                                     _var17)))
                       (if _var16
                           (let ((_var19 (makeLeaf k
                                                   e)))
                                _var19)
                           (let ((_var20 (car t)))
                                (let ((_var21 (car _var20)))
                                     (let ((_var23 "node"))
                                          (let ((_var22 (eq? _var21
                                                             _var23)))
                                               (if _var22
                                                   (let ((_var25 (insertTNode k
                                                                              e
                                                                              t)))
                                                        _var25)
                                                   (let ((_var26 (insertTLeaf k
                                                                              e
                                                                              t)))
                                                        _var26)))))))))))

(define (insertTLeaf k e t)
        (let ((_var27 (car t)))
             (let ((key (cdr _var27)))
                  (let ((_var29 (< k
                                   key)))
                       (if _var29
                           (let ((_var30 (makeLeaf k
                                                   e)))
                                (let ((_var31 (makeNode k
                                                        _var30
                                                        t)))
                                     _var31))
                           (let ((_var32 (> k
                                            key)))
                                (if _var32
                                    (let ((_var33 (makeLeaf k
                                                            e)))
                                         (let ((_var34 (makeNode key
                                                                 t
                                                                 _var33)))
                                              _var34))
                                    (let ((_var35 99))
                                         (let ((_var37 99))
                                              (let ((_var36 (cons _var35
                                                                  _var37)))
                                                   _var36))))))))))

(define (insertTNode k e t)
        (let ((_var39 (car t)))
             (let ((key (cdr _var39)))
                  (let ((_var41 (cdr t)))
                       (let ((tree1 (car _var41)))
                            (let ((_var43 (cdr t)))
                                 (let ((_var44 (cdr _var43)))
                                      (let ((tree2 (car _var44)))
                                           (let ((_var46 (> k
                                                            key)))
                                                (if _var46
                                                    (let ((_var47 (insertT k
                                                                           e
                                                                           tree2)))
                                                         (let ((_var48 (makeNode key
                                                                                 tree1
                                                                                 _var47)))
                                                              _var48))
                                                    (let ((_var49 (insertT k
                                                                           e
                                                                           tree1)))
                                                         (let ((_var50 (makeNode key
                                                                                 _var49
                                                                                 tree2)))
                                                              _var50))))))))))))

(define (lookupT k t)
        (let ((_var51 (car t)))
             (let ((_var52 (car _var51)))
                  (let ((_var54 "empty"))
                       (let ((_var53 (eq? _var52
                                          _var54)))
                            (if _var53
                                (let ((_var56 '()))
                                     _var56)
                                (let ((_var57 (car t)))
                                     (let ((_var58 (car _var57)))
                                          (let ((_var60 "node"))
                                               (let ((_var59 (eq? _var58
                                                                  _var60)))
                                                    (if _var59
                                                        (let ((_var62 (lookupTNode k
                                                                                   t)))
                                                             _var62)
                                                        (let ((_var63 (lookupTLeaf k
                                                                                   t)))
                                                             _var63))))))))))))

(define (lookupTLeaf k t)
        (let ((_var64 (car t)))
             (let ((key (cdr _var64)))
                  (let ((_var66 (cdr t)))
                       (let ((e (car _var66)))
                            (let ((_var68 (eq? k
                                               key)))
                                 (if _var68
                                     e
                                     (let ((_var69 '()))
                                          _var69))))))))

(define (lookupTNode k t)
        (let ((_var70 (car t)))
             (let ((key (cdr _var70)))
                  (let ((_var72 (cdr t)))
                       (let ((tree1 (car _var72)))
                            (let ((_var74 (cdr t)))
                                 (let ((_var75 (cdr _var74)))
                                      (let ((tree2 (car _var75)))
                                           (let ((_var77 (> k
                                                            key)))
                                                (if _var77
                                                    (let ((_var78 (lookupT k
                                                                           tree2)))
                                                         _var78)
                                                    (let ((_var79 (lookupT k
                                                                           tree1)))
                                                         _var79)))))))))))

(define (forceTree t)
        (let ((_var80 ()))
             _var80))

(define (readTree n t)
        (let ((_var81 0))
             (let ((_var82 (eq? _var81
                                n)))
                  (if _var82
                      t
                      (let ((en (makeEntity n)))
                           (let ((_var83 (car en)))
                                (let ((_var85 (cdr en)))
                                     (let ((_var86 (car _var85)))
                                          (let ((_var88 (cdr en)))
                                               (let ((_var89 (cdr _var88)))
                                                    (let ((_var90 (car _var89)))
                                                         (let ((_var92 '()))
                                                              (let ((_var91 (cons _var90
                                                                                  _var92)))
                                                                   (let ((_var87 (cons _var86
                                                                                       _var91)))
                                                                        (let ((e (cons _var83
                                                                                       _var87)))
                                                                             (let ((k (makeKey e)))
                                                                                  (let ((_var96 3))
                                                                                       (let ((_var97 (- n
                                                                                                        _var96)))
                                                                                            (let ((_var99 (insertT k
                                                                                                                   e
                                                                                                                   t)))
                                                                                                 (let ((_var98 (readTree _var97
                                                                                                                         _var99)))
                                                                                                      _var98))))))))))))))))))))

(define (makeEntity n)
        (let ((_var101 2))
             (let ((f (- n
                         _var101)))
                  (let ((_var103 1))
                       (let ((g (- n
                                   _var103)))

                                 (let ((_var105 ()))
                                      (let ((_var106 (cons n
                                                           _var105)))
                                           (let ((_var107 (cons g
                                                                _var106)))
                                                (let ((_var108 (cons f
                                                                     _var107)))
                                                     _var108)))))))))

(define (makeKey e)
        (let ((_var109 (car e)))
             _var109))

(define (join t1 t2 tj)
        (let ((_var110 (car t1)))
             (let ((_var112 "empty"))
                  (let ((_var111 (eq? _var110
                                      _var112)))
                       (let ((_var115 (car t2)))
                            (let ((_var117 "empty"))
                                 (let ((_var116 (eq? _var115
                                                     _var117)))
                                      (let ((_var114 (or _var111
                                                         _var116)))
                                           (if _var114
                                               tj
                                               (let ((_var120 (car t1)))
                                                    (let ((_var121 (car _var120)))
                                                         (let ((_var123 "node"))
                                                              (let ((_var122 (eq? _var121
                                                                                  _var123)))
                                                                   (if _var122
                                                                       (let ((_var125 (cdr t1)))
                                                                            (let ((tree1 (car _var125)))
                                                                                 (let ((_var127 (cdr t1)))
                                                                                      (let ((_var128 (cdr _var127)))
                                                                                           (let ((tree2 (car _var128)))
                                                                                                (let ((_var130 (join tree2
                                                                                                                     t2
                                                                                                                     tj)))
                                                                                                     (let ((_var131 (join tree1
                                                                                                                          t2
                                                                                                                          _var130)))
                                                                                                          _var131)))))))
                                                                       (let ((_var132 (car t1)))
                                                                            (let ((key (cdr _var132)))
                                                                                 (let ((_var134 (cdr t1)))
                                                                                      (let ((e (car _var134)))
                                                                                           (let ((a (car e)))
                                                                                                (let ((_var136 (cdr e)))
                                                                                                     (let ((b (car _var136)))
                                                                                                          (let ((_var138 (cdr e)))
                                                                                                               (let ((_var139 (cdr _var138)))
                                                                                                                    (let ((c (car _var139)))
                                                                                                                         (let ((entity (lookupT a
                                                                                                                                                t2)))
                                                                                                                              (let ((_var141 (null? entity)))
                                                                                                                                   (if _var141
                                                                                                                                       tj
                                                                                                                                       (let ((d (car entity)))
                                                                                                                                            (let ((_var142 (cdr entity)))
                                                                                                                                                 (let ((f (car _var142)))
                                                                                                                                                      (let ((_var144 '()))
                                                                                                                                                           (let ((_var145 (cons f
                                                                                                                                                                                _var144)))
                                                                                                                                                                (let ((_var146 (cons d
                                                                                                                                                                                     _var145)))
                                                                                                                                                                     (let ((_var147 (cons c
                                                                                                                                                                                          _var146)))
                                                                                                                                                                          (let ((_var148 (cons b
                                                                                                                                                                                               _var147)))
                                                                                                                                                                               (let ((newe (cons a
                                                                                                                                                                                                 _var148)))
                                                                                                                                                                                    (let ((_var150 (insertT c
                                                                                                                                                                                                            newe
                                                                                                                                                                                                            tj)))
                                                                                                                                                                                         _var150)))))))))))))))))))))))))))))))))))))

(define (main abc)
        (let ((_var151 0))
             (let ((_var153 0))
                  (let ((_var152 (eq? _var151
                                      _var153)))
                       (if _var152
                           (let ((_var155 6))
                                (let ((_var157 "empty"))
                                     (let ((_var159 ()))
                                          (let ((_var158 (cons _var157
                                                               _var159)))
                                               (let ((a (readTree _var155
                                                                  _var158)))
                                                    (let ((_var162 15))
                                                         (let ((_var164 "empty"))
                                                              (let ((_var166 '()))
                                                                   (let ((_var165 (cons _var164
                                                                                        _var166)))
                                                                        (let ((b (readTree _var162
                                                                                           _var165)))
                                                                             (let ((_var169 "empty"))
                                                                                  (let ((_var171 ()))
                                                                                       (let ((_var170 (cons _var169
                                                                                                            _var171)))
                                                                                            (let ((_var173 (join a
                                                                                                                 b
                                                                                                                 _var170)))
                                                                                                 _var173))))))))))))))
                           (let ((_var174 9))
                                (let ((_var176 9))
                                     (let ((_var175 (cons _var174
                                                          _var176)))
                                          _var175))))))))
(let ((_var179 1))
(let ((_var178 (main _var179)))
     _var178))
