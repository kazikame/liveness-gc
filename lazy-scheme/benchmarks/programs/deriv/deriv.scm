(define (equal? a b)
        (let ((_var0 (eq? a
                          b)))
             _var0))

(define (not x)
        (if x
            (let ((_var1 #f))
                 _var1)
            (let ((_var2 #t))
                 _var2)))

(define (myderiv a)
        (let ((_var3 (null? a)))
             (if _var3
                 (let ((_var4 '()))
                      _var4)
                 (let ((_var5 (car a)))
                      (let ((_var6 (deriv _var5)))
                           (let ((_var8 (cdr a)))
                                (let ((_var9 (myderiv _var8)))
                                     (let ((_var7 (cons _var6
                                                        _var9)))
                                          _var7))))))))

(define (myderiv-aux a)
        (let ((_var11 (null? a)))
             (if _var11
                 (let ((_var12 '()))
                      _var12)
                 (let ((_var13 (car a)))
                      (let ((_var14 (deriv-aux _var13)))
                           (let ((_var16 (cdr a)))
                                (let ((_var17 (myderiv-aux _var16)))
                                     (let ((_var15 (cons _var14
                                                         _var17)))
                                          _var15))))))))

(define (list1 x)
        (let ((_var13 '()))
             (let ((_var14 (cons x
                                 _var13)))
                  _var14)))

(define (list2 x y)
        (let ((_var15 (list1 y)))
             (let ((_var16 (cons x
                                 _var15)))
                  _var16)))

(define (list3 x y z)
        (let ((_var17 (list2 y
                             z)))
             (let ((_var18 (cons x
                                 _var17)))
                  _var18)))

(define (list4 x y z w)
        (let ((_var19 (list3 y
                             z
                             w)))
             (let ((_var20 (cons x
                                 _var19)))
                  _var20)))

(define (list5 x y z w v)
        (let ((_var29 (list4 y
                             z
                             w
                             v)))
             (let ((_var30 (cons x
                                 _var29)))
                  _var30)))

(define (deriv-aux a)
        (let ((_var19 "/"))
             (let ((_var21 (deriv a)))
                  (let ((_var20 (list3 _var19
                                       _var21
                                       a)))
                       _var20))))

(define (caddr a)
        (let ((_var23 (cdr a)))
             (let ((_var24 (cdr _var23)))
                  (let ((_var25 (car _var24)))
                       _var25))))

(define (cadr a)
        (let ((_var26 (cdr a)))
             (let ((_var27 (car _var26)))
                  _var27)))

(define (deriv a)
        (let ((_var28 (pair? a)))
             (if _var28
                 (let ((_var29 (car a)))
                      (let ((_var31 "+"))
                           (let ((_var30 (equal? _var29
                                                 _var31)))
                                (if _var30
                                    (let ((_var33 "+"))
                                         (let ((_var35 (cdr a)))
                                              (let ((_var36 (myderiv _var35)))
                                                   (let ((_var34 (cons _var33
                                                                       _var36)))
                                                        _var34))))
                                    (let ((_var38 (car a)))
                                         (let ((_var40 "-"))
                                              (let ((_var39 (equal? _var38
                                                                    _var40)))
                                                   (if _var39
                                                       (let ((_var42 "-"))
                                                            (let ((_var44 (cdr a)))
                                                                 (let ((_var45 (myderiv _var44)))
                                                                      (let ((_var43 (cons _var42
                                                                                          _var45)))
                                                                           _var43))))
                                                       (let ((_var47 (car a)))
                                                            (let ((_var49 "*"))
                                                                 (let ((_var48 (equal? _var47
                                                                                       _var49)))
                                                                      (if _var48
                                                                          (let ((_var51 "*"))
                                                                               (let ((_var53 "+"))
                                                                                    (let ((_var55 (cdr a)))
                                                                                         (let ((_var56 (myderiv-aux _var55)))
                                                                                              (let ((_var54 (cons _var53
                                                                                                                  _var56)))
                                                                                                   (let ((_var52 (list3 _var51
                                                                                                                        a
                                                                                                                        _var54)))
                                                                                                        _var52))))))
                                                                          (let ((_var59 (car a)))
                                                                               (let ((_var61 "/"))
                                                                                    (let ((_var60 (equal? _var59
                                                                                                          _var61)))
                                                                                         (if _var60
                                                                                             (let ((_var63 "-"))
                                                                                                  (let ((_var65 "/"))
                                                                                                       (let ((_var67 (cadr a)))
                                                                                                            (let ((_var68 (deriv _var67)))
                                                                                                                 (let ((_var70 (caddr a)))
                                                                                                                      (let ((_var66 (list3 _var65
                                                                                                                                           _var68
                                                                                                                                           _var70)))
                                                                                                                           (let ((_var73 "/"))
                                                                                                                                (let ((_var75 (cadr a)))
                                                                                                                                     (let ((_var77 "*"))
                                                                                                                                          (let ((_var79 (caddr a)))
                                                                                                                                               (let ((_var81 (caddr a)))
                                                                                                                                                    (let ((_var83 (caddr a)))
                                                                                                                                                         (let ((_var84 (deriv _var83)))
                                                                                                                                                              (let ((_var78 (list3 _var77
                                                                                                                                                                                   _var79
                                                                                                                                                                                   _var81
                                                                                                                                                                                   _var84)))
                                                                                                                                                                   (let ((_var74 (list3 _var73
                                                                                                                                                                                        _var75
                                                                                                                                                                                        _var78)))
                                                                                                                                                                        (let ((_var64 (list3 _var63
                                                                                                                                                                                             _var66
                                                                                                                                                                                             _var74)))
                                                                                                                                                                             _var64))))))))))))))))
                                                                                             (let ((_var88 '()))
                                                                                                  _var88)))))))))))))))))
                 (let ((_var89 "x"))
                      (let ((_var90 (equal? a
                                            _var89)))
                           (if _var90
                               (let ((_var91 1))
                                    _var91)
                               (let ((_var92 0))
                                    _var92)))))))

(let ((_var93 "+"))
     (let ((_var95 "*"))
          (let ((_var97 3))
               (let ((_var99 "x"))
                    (let ((_var101 "x"))
                         (let ((_var96 (list4 _var95
                                              _var97
                                              _var99
                                              _var101)))
                              (let ((_var104 "*"))
                                   (let ((_var106 "a"))
                                        (let ((_var108 "x"))
                                             (let ((_var110 "x"))
                                                  (let ((_var105 (list4 _var104
                                                                        _var106
                                                                        _var108
                                                                        _var110)))
                                                       (let ((_var113 "*"))
                                                            (let ((_var115 "b"))
                                                                 (let ((_var117 "x"))
                                                                      (let ((_var114 (list3 _var113
                                                                                            _var115
                                                                                            _var117)))
                                                                           (let ((_var120 5))
                                                                                (let ((_var94 (list5 _var93
                                                                                                     _var96
                                                                                                     _var105
                                                                                                     _var114
                                                                                                     _var120)))
                                                                                     (let ((_var122 (deriv _var94)))
                                                                                          _var122))))))))))))))))))