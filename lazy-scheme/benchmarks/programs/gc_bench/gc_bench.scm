(define (or a b)
  (if a
      a
      b))

(define (divide a b)
        (let ((_var0 0))
             (let ((_var1 (divide1 a
                                   b
                                   _var0)))
                  _var1)))

(define (divide1 a b i)
        (let ((_var2 (< a
                        b)))
             (if _var2
                 i
                 (let ((_var3 (- a
                                 b)))
                      (let ((_var5 1))
                           (let ((_var6 (+ i
                                           _var5)))
                                (let ((_var4 (divide1 _var3
                                                      b
                                                      _var6)))
                                     _var4)))))))

(define (Node left right)
        (let ((_var8 (cons left
                           right)))
             _var8))



(define (Power2 i)
        (let ((_var9 1))
             (let ((_var10 (eq? _var9
                                i)))
                  (if _var10
                      (let ((_var11 2))
                           _var11)
                      (let ((_var12 2))
                           (let ((_var14 1))
                                (let ((_var15 (- i
                                                 _var14)))
                                     (let ((_var16 (Power2 _var15)))
                                          (let ((_var13 (* _var12
                                                           _var16)))
                                               _var13)))))))))

(define (TreeSize i)
        (let ((_var18 1))
             (let ((_var19 (+ i
                              _var18)))
                  (let ((_var20 (Power2 _var19)))
                       (let ((_var22 1))
                            (let ((_var21 (- _var20
                                             _var22)))
                                 _var21))))))

(define (NumIters i kStretchTreeDepth)
        (let ((_var24 2))
             (let ((_var26 (TreeSize kStretchTreeDepth)))
                  (let ((_var28 (TreeSize i)))
                       (let ((_var27 (divide _var26
                                             _var28)))
                            (let ((_var25 (* _var24
                                             _var27)))
                                 _var25))))))

(define (MakeTree iDepth)
        (let ((_var31 0))
             (let ((_var32 (eq? _var31
                                iDepth)))
                  (if _var32
                      (let ((_var33 '()))
                           (let ((_var35 '()))
                                (let ((_var34 (Node _var33
                                                    _var35)))
                                     _var34)))
                      (let ((_var37 1))
                           (let ((_var38 (- iDepth
                                            _var37)))
                                (let ((_var39 (MakeTree _var38)))
                                     (let ((_var41 1))
                                          (let ((_var42 (- iDepth
                                                           _var41)))
                                               (let ((_var43 (MakeTree _var42)))
                                                    (let ((_var40 (Node _var39
                                                                        _var43)))
                                                         _var40)))))))))))

(define (BottomUp depth)
        (let ((_var45 (MakeTree depth)))
             _var45))

(define (LoopBottomUp i depth)
        (let ((_var46 0))
             (let ((_var47 (eq? i
                                _var46)))
                  (if _var47
                      (let ((_var48 '()))
                           _var48)
                      (let ((tempTree (MakeTree depth)))
                           (let ((tempTree '()))
                                (let ((_var49 1))
                                     (let ((_var50 (- i
                                                      _var49)))
                                          (let ((_var51 (LoopBottomUp _var50
                                                                      depth)))
                                               _var51)))))))))

(define (TimeConstruction depth kStretchTreeDepth)
        (let ((iNumIters (NumIters depth
                                   kStretchTreeDepth)))
             (let ((_var52 (LoopBottomUp iNumIters
                                         depth)))
                  _var52)))

(define (loopTimeConstruction kMinTreeDepth kMaxTreeDepth kStretchTreeDepth)
        (let ((_var53 (> kMinTreeDepth
                         kMaxTreeDepth)))
             (if _var53
                 (let ((_var54 '()))
                      _var54)
                 (let ((iter (TimeConstruction kMinTreeDepth
                                               kStretchTreeDepth)))
                      (let ((_var55 2))
                           (let ((_var56 (+ kMinTreeDepth
                                            _var55)))
                                (let ((_var57 (loopTimeConstruction _var56
                                                                    kMaxTreeDepth
                                                                    kStretchTreeDepth)))
                                     _var57)))))))

(define (createArray kArraySize i)
        (let ((_var58 (eq? i
                           kArraySize)))
             (if _var58
                 (let ((_var59 '()))
                      (let ((_var61 '()))
                           (let ((_var60 (cons _var59
                                               _var61)))
                                _var60)))
                 (let ((_var63 2))
                      (let ((_var64 (divide kArraySize
                                            _var63)))
                           (let ((_var65 (> i
                                            _var64)))
                                (if _var65
                                    (let ((_var66 '()))
                                         (let ((_var68 1))
                                              (let ((_var69 (+ i
                                                               _var68)))
                                                   (let ((_var70 (createArray kArraySize
                                                                              _var69)))
                                                        (let ((_var67 (cons _var66
                                                                            _var70)))
                                                             _var67)))))
                                    (let ((_var72 1))
                                         (let ((_var73 (+ i
                                                          _var72)))
                                              (let ((_var74 (createArray kArraySize
                                                                         _var73)))
                                                   (let ((_var75 (cons i
                                                                       _var74)))
                                                        _var75)))))))))))

(define (checkArray array index)
        (let ((_var76 1))
             (let ((_var77 (eq? index
                                _var76)))
                  (if _var77
                      (let ((_var78 (car array)))
                           _var78)
                      (let ((_var79 (cdr array)))
                           (let ((_var81 1))
                                (let ((_var82 (- index
                                                 _var81)))
                                     (let ((_var80 (checkArray _var79
                                                               _var82)))
                                          _var80))))))))

(define (main kStretchTreeDepth kLongLivedTreeDepth kArraySize kMinTreeDepth kMaxTreeDepth)
        (let ((tempTree (MakeTree kStretchTreeDepth)))
             (let ((tempTree '()))
                  (let ((longLivedTree (MakeTree kLongLivedTreeDepth)))
                       (let ((_var84 1))
                            (let ((array (createArray kArraySize
                                                      _var84)))
                                 (let ((loopTC (loopTimeConstruction kMinTreeDepth
                                                                     kMaxTreeDepth
                                                                     kStretchTreeDepth)))
                                      (let ((_var86 (null? longLivedTree)))
                                           (let ((_var88 #f))
                                                (let ((_var90 1))
                                                     (let ((_var91 (checkArray array
                                                                               _var90)))
                                                          (let ((_var93 1))
                                                               (let ((_var92 (eq? _var91
                                                                                  _var93)))
                                                                    (let ((_var89 (eq? _var88
                                                                                       _var92)))
                                                                         (let ((_var87 (or _var86
                                                                                           _var89)))
                                                                              (if _var87
                                                                                  (let ((_var97 '()))
                                                                                       _var97)
                                                                                  (let ((_var98 50))
                                                                                       (let ((_var100 (createTreeLengthList kLongLivedTreeDepth _var98)))
                                                                                            (let ((_var99 (cons _var98
                                                                                                                _var100)))
                                                                                                 _var99)))))))))))))))))))



(define (touchNodes Tree) 
  (let ((isnulltree (null? Tree)))
  (let ((c (not isnulltree)))
    (if isnulltree
	(let ((ret 0))
	  ret)
	(let ((ltree (car Tree)))
	  (let ((lsum (touchNodes ltree)))
	    (let ((rtree (cdr Tree)))
	      (let ((rsum (touchNodes rtree)))
		(let ((treesum (+ lsum rsum)))
		  (let ((one 1))
		    (let ((res (+ one treesum)))
		      res)))))))))))

(define (createTreeLengthList d i)
  (let ((j 0))
    (let ((c (eq? i j)))
      (if c
	  (let ((ret '()))
	    ret)
	  (let ((Tree (MakeTree d)))
	    (let ((hd (touchNodes Tree)))
	      (let ((one 1))
		(let ((k (- i one)))
		  (let ((tl (createTreeLengthList d k)))
		    (let ((res (cons hd tl)))
		      res))))))))))

;; (define (sum l)
;;   (let ((nil (null? l)))
;;     (if nil 
;; 	(let ((r 0))
;; 	  r)
;; 	(let ((hd1 (car l)))
;; 	  (let ((tl1 (cdr l)))
;; 	    (let ((tlsum (sum tl1)))
;; 	      (let ((ans (+ hd1 tlsum)))
;; 		ans)))))))

;; (define (sumTreeSizeList T)
;;   (let ((i1 10))
;;     (let ((tsizelst (createTreeLengthList T i1)))
;;       (let ((tsum (sum tsizelst)))
;; 	tsum))))
  

(let ((_var102 16))
     (let ((_var104 10))
          (let ((_var106 5))
               (let ((_var108 4))
                    (let ((_var110 10))
                         (let ((_var103 (main _var102
                                              _var104
                                              _var106
                                              _var108
                                              _var110)))
                              _var103))))))