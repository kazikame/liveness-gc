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

(define (valid vs vp)
        (let ((_var6 (null? vs)))
             (if _var6
                 (let ((_var7 #t))
                      _var7)
                 (let ((vf (car vs)))
                      (let ((_var8 (car vf)))
                           (let ((_var10 (car vp)))
                                (let ((_var9 (eq? _var8
                                                  _var10)))
                                     (if _var9
                                         (let ((_var12 #f))
                                              _var12)
                                         (let ((_var13 (cdr vf)))
                                              (let ((_var15 (cdr vp)))
                                                   (let ((_var14 (eq? _var13
                                                                      _var15)))
                                                        (if _var14
                                                            (let ((_var17 #f))
                                                                 _var17)
                                                            (let ((_var18 (car vf)))
                                                                 (let ((_var20 (car vp)))
                                                                      (let ((rd (- _var18
                                                                                   _var20)))
                                                                           (let ((_var22 (cdr vf)))
                                                                                (let ((_var24 (cdr vp)))
                                                                                     (let ((cd (- _var22
                                                                                                  _var24)))
                                                                                          (let ((_var26 (- rd
                                                                                                           cd)))
                                                                                               (let ((_var28 0))
                                                                                                    (let ((_var27 (eq? _var26
                                                                                                                       _var28)))
                                                                                                         (if _var27
                                                                                                             (let ((_var30 #f))
                                                                                                                  _var30)
                                                                                                             (let ((_var31 (+ rd
                                                                                                                              cd)))
                                                                                                                  (let ((_var33 0))
                                                                                                                       (let ((_var32 (eq? _var31
                                                                                                                                          _var33)))
                                                                                                                            (if _var32
                                                                                                                                (let ((_var35 #f))
                                                                                                                                     _var35)
                                                                                                                                (let ((_var36 (cdr vs)))
                                                                                                                                     (let ((_var37 (valid _var36
                                                                                                                                                          vp)))
                                                                                                                                          _var37))))))))))))))))))))))))))))

(define (addto ats atps)
        (let ((_var38 (null? atps)))
             (if _var38
                 (let ((_var39 '()))
                      _var39)
                 (let ((_var40 (car atps)))
                      (let ((_var41 (valid ats
                                           _var40)))
                           (if _var41
                               (let ((_var42 (car atps)))
                                    (let ((_var44 '()))
                                         (let ((_var43 (cons _var42
                                                             _var44)))
                                              (let ((_var46 (append ats
                                                                    _var43)))
                                                   (let ((_var48 '()))
                                                        (let ((_var47 (cons _var46
                                                                            _var48)))
                                                             (let ((_var51 (cdr atps)))
                                                                  (let ((_var52 (addto ats
                                                                                       _var51)))
                                                                       (let ((_var50 (append _var47
                                                                                             _var52)))
                                                                            _var50)))))))))
                               (let ((_var54 (cdr atps)))
                                    (let ((_var55 (addto ats
                                                         _var54)))
                                         _var55))))))))

(define (tries tn tk)
        (let ((_var56 0))
             (let ((_var57 (eq? tn
                                _var56)))
                  (if _var57
                      (let ((_var58 '()))
                           _var58)
                      (let ((_var59 1))
                           (let ((_var60 (- tn
                                            _var59)))
                                (let ((_var61 (tries _var60
                                                     tk)))
                                     (let ((_var63 (cons tk
                                                         tn)))
                                          (let ((_var65 '()))
                                               (let ((_var64 (cons _var63
                                                                   _var65)))
                                                    (let ((_var62 (append _var61
                                                                          _var64)))
                                                         _var62)))))))))))

(define (extend ess en ek)
        (let ((_var68 (null? ess)))
             (if _var68
                 (let ((_var69 '()))
                      _var69)
                 (let ((_var70 (car ess)))
                      (let ((_var72 (tries en
                                           ek)))
                           (let ((_var71 (addto _var70
                                                _var72)))
                                (let ((_var75 (cdr ess)))
                                     (let ((_var76 (extend _var75
                                                           en
                                                           ek)))
                                          (let ((_var74 (append _var71
                                                                _var76)))
                                               _var74)))))))))

(define (base bn)
        (let ((_var78 0))
             (let ((_var79 (eq? bn
                                _var78)))
                  (if _var79
                      (let ((_var80 '()))
                           _var80)
                      (let ((_var81 1))
                           (let ((_var82 (- bn
                                            _var81)))
                                (let ((_var83 (base _var82)))
                                     (let ((_var85 1))
                                          (let ((_var86 (cons _var85
                                                              bn)))
                                               (let ((_var88 '()))
                                                    (let ((_var87 (cons _var86
                                                                        _var88)))
                                                         (let ((_var91 '()))
                                                              (let ((_var90 (cons _var87
                                                                                  _var91)))
                                                                   (let ((_var84 (append _var83
                                                                                         _var90)))
                                                                        _var84))))))))))))))

(define (solve sn sk)
        (let ((_var94 1))
             (let ((_var95 (eq? sk
                                _var94)))
                  (if _var95
                      (let ((_var96 (base sn)))
                           _var96)
                      (let ((_var97 1))
                           (let ((_var98 (- sk
                                            _var97)))
                                (let ((ss (solve sn
                                                 _var98)))
                                     (let ((_var100 (extend ss
                                                            sn
                                                            sk)))
                                          _var100))))))))

(define (nqueens nqn)
        (let ((_var101 (solve nqn
                              nqn)))
             _var101))


(define (wrapper n l)
  (let ((zero 0))
    (let ((c (> n zero)))
      (if c
	  (let ((one 1))
	    (let ((m (- n one)))
	      (let ((tl (wrapper m l)))
		(let ((k 8))
		  (let ((hd (nqueens k)))
		    (let ((res (cons hd tl)))
		      res))))))
	  l
	  ))))

(let ((_var102 9))
  (let ((l '()))
     (let ((_var103 (nqueens _var102)))
          _var103)))
