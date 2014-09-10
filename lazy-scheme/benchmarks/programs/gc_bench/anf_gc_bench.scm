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

(define (divide a b)
        (let ((_var5 0))
             (let ((_var6 (divide1 a
                                   b
                                   _var5)))
                  _var6)))

(define (divide1 a b i)
        (let ((_var7 (< a
                        b)))
             (if _var7
                 i
                 (let ((_var8 (- a
                                 b)))
                      (let ((_var10 1))
                           (let ((_var11 (+ i
                                            _var10)))
                                (let ((_var9 (divide1 _var8
                                                      b
                                                      _var11)))
                                     _var9)))))))

(define (Node left right)
        (let ((_var13 (cons left
                            right)))
             _var13))

(define (Power2 i)
        (let ((_var14 1))
             (let ((_var15 (eq? _var14
                                i)))
                  (if _var15
                      (let ((_var16 2))
                           _var16)
                      (let ((_var17 2))
                           (let ((_var19 1))
                                (let ((_var20 (- i
                                                 _var19)))
                                     (let ((_var21 (Power2 _var20)))
                                          (let ((_var18 (* _var17
                                                           _var21)))
                                               _var18)))))))))

(define (TreeSize i)
        (let ((_var23 1))
             (let ((_var24 (+ i
                              _var23)))
                  (let ((_var25 (Power2 _var24)))
                       (let ((_var27 1))
                            (let ((_var26 (- _var25
                                             _var27)))
                                 _var26))))))

(define (NumIters i kStretchTreeDepth)
        (let ((_var29 2))
             (let ((_var31 (TreeSize kStretchTreeDepth)))
                  (let ((_var33 (TreeSize i)))
                       (let ((_var32 (divide _var31
                                             _var33)))
                            (let ((_var30 (* _var29
                                             _var32)))
                                 _var30))))))

(define (MakeTree iDepth)
        (let ((_var36 0))
             (let ((_var37 (eq? _var36
                                iDepth)))
                  (if _var37
                      (let ((_var38 '()))
                           (let ((_var40 '()))
                                (let ((_var39 (Node _var38
                                                    _var40)))
                                     _var39)))
                      (let ((_var42 1))
                           (let ((_var43 (- iDepth
                                            _var42)))
                                (let ((_var44 (MakeTree _var43)))
                                     (let ((_var46 1))
                                          (let ((_var47 (- iDepth
                                                           _var46)))
                                               (let ((_var48 (MakeTree _var47)))
                                                    (let ((_var45 (Node _var44
                                                                        _var48)))
                                                         _var45)))))))))))

(define (BottomUp depth)
        (let ((_var50 (MakeTree depth)))
             _var50))

(define (LoopBottomUp i depth)
        (let ((_var51 0))
             (let ((_var52 (eq? i
                                _var51)))
                  (if _var52
                      (let ((_var53 '()))
                           _var53)
                      (let ((tempTree (MakeTree depth)))
                           (let ((tempTree '()))
                                (let ((_var54 1))
                                     (let ((_var55 (- i
                                                      _var54)))
                                          (let ((_var56 (LoopBottomUp _var55
                                                                      depth)))
                                               _var56)))))))))

(define (TimeConstruction depth kStretchTreeDepth)
        (let ((iNumIters (NumIters depth
                                   kStretchTreeDepth)))
             (let ((_var57 (LoopBottomUp iNumIters
                                         depth)))
                  _var57)))

(define (loopTimeConstruction kMinTreeDepth kMaxTreeDepth kStretchTreeDepth)
        (let ((_var58 (> kMinTreeDepth
                         kMaxTreeDepth)))
             (if _var58
                 (let ((_var59 '()))
                      _var59)
                 (let ((iter (TimeConstruction kMinTreeDepth
                                               kStretchTreeDepth)))
                      (let ((_var60 2))
                           (let ((_var61 (+ kMinTreeDepth
                                            _var60)))
                                (let ((_var62 (loopTimeConstruction _var61
                                                                    kMaxTreeDepth
                                                                    kStretchTreeDepth)))
                                     _var62)))))))

(define (createArray kArraySize i)
        (let ((_var63 (eq? i
                           kArraySize)))
             (if _var63
                 (let ((_var64 '()))
                      (let ((_var66 '()))
                           (let ((_var65 (cons _var64
                                               _var66)))
                                _var65)))
                 (let ((_var68 2))
                      (let ((_var69 (divide kArraySize
                                            _var68)))
                           (let ((_var70 (> i
                                            _var69)))
                                (if _var70
                                    (let ((_var71 '()))
                                         (let ((_var73 1))
                                              (let ((_var74 (+ i
                                                               _var73)))
                                                   (let ((_var75 (createArray kArraySize
                                                                              _var74)))
                                                        (let ((_var72 (cons _var71
                                                                            _var75)))
                                                             _var72)))))
                                    (let ((_var77 1))
                                         (let ((_var78 (+ i
                                                          _var77)))
                                              (let ((_var79 (createArray kArraySize
                                                                         _var78)))
                                                   (let ((_var80 (cons i
                                                                       _var79)))
                                                        _var80)))))))))))

(define (checkArray array index)
        (let ((_var81 1))
             (let ((_var82 (eq? index
                                _var81)))
                  (if _var82
                      (let ((_var83 (car array)))
                           _var83)
                      (let ((_var84 (cdr array)))
                           (let ((_var86 1))
                                (let ((_var87 (- index
                                                 _var86)))
                                     (let ((_var85 (checkArray _var84
                                                               _var87)))
                                          _var85))))))))

(define (main kStretchTreeDepth kLongLivedTreeDepth kArraySize kMinTreeDepth kMaxTreeDepth)
        (let ((tempTree (MakeTree kStretchTreeDepth)))
             (let ((tempTree '()))
                  (let ((longLivedTree (MakeTree kLongLivedTreeDepth)))
                       (let ((_var89 1))
                            (let ((array (createArray kArraySize
                                                      _var89)))
                                 (let ((loopTC (loopTimeConstruction kMinTreeDepth
                                                                     kMaxTreeDepth
                                                                     kStretchTreeDepth)))
                                      (let ((_var91 (null? longLivedTree)))
                                           (let ((_var93 #f))
                                                (let ((_var95 1))
                                                     (let ((_var96 (checkArray array
                                                                               _var95)))
                                                          (let ((_var98 1))
                                                               (let ((_var97 (eq? _var96
                                                                                  _var98)))
                                                                    (let ((_var94 (eq? _var93
                                                                                       _var97)))
                                                                         (let ((_var92 (or _var91
                                                                                           _var94)))
                                                                              (if _var92
                                                                                  (let ((_var102 '()))
                                                                                       _var102)
                                                                                  (let ((_var103 100))
                                                                                       (let ((_var105 100))
                                                                                            (let ((_var104 (cons _var103
                                                                                                                 _var105)))
                                                                                                 _var104)))))))))))))))))))

(let ((_var107 16))
     (let ((_var109 10))
          (let ((_var111 5))
               (let ((_var113 4))
                    (let ((_var115 10))
                         (let ((_var108 (main _var107
                                              _var109
                                              _var111
                                              _var113
                                              _var115)))
                              _var108))))))