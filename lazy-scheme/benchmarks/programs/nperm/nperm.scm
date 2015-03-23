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

(define (append res1 res2)
        (let ((_var5 (null? res1)))
             (if _var5
                 res2
                 (let ((_var6 (car res1)))
                      (let ((_var8 (cdr res1)))
                           (let ((_var9 (append _var8
                                                res2)))
                                (let ((_var7 (cons _var6
                                                   _var9)))
                                     _var7)))))))

(define (getVal list i)
        (let ((_var11 0))
             (let ((_var12 (eq? _var11
                                i)))
                  (if _var12
                      (let ((_var13 (car list)))
                           _var13)
                      (let ((_var14 (cdr list)))
                           (let ((_var16 1))
                                (let ((_var17 (- i
                                                 _var16)))
                                     (let ((_var15 (getVal _var14
                                                           _var17)))
                                          _var15))))))))

(define (swapij list i j ival jval)
        (let ((_var19 0))
             (let ((_var20 (< i
                              _var19)))
                  (let ((_var22 0))
                       (let ((_var23 (< j
                                        _var22)))
                            (let ((_var21 (and _var20
                                               _var23)))
                                 (if _var21
                                     list
                                     (let ((_var25 0))
                                          (let ((_var26 (eq? _var25
                                                             i)))
                                               (if _var26
                                                   (let ((_var27 (cdr list)))
                                                        (let ((_var29 1))
                                                             (let ((_var30 (- i
                                                                              _var29)))
                                                                  (let ((_var32 1))
                                                                       (let ((_var33 (- j
                                                                                        _var32)))
                                                                            (let ((_var28 (swapij _var27
                                                                                                  _var30
                                                                                                  _var33
                                                                                                  ival
                                                                                                  jval)))
                                                                                 (let ((_var35 (cons jval
                                                                                                     _var28)))
                                                                                      _var35)))))))
                                                   (let ((_var36 0))
                                                        (let ((_var37 (eq? _var36
                                                                           j)))
                                                             (if _var37
                                                                 (let ((_var38 (cdr list)))
                                                                      (let ((_var40 1))
                                                                           (let ((_var41 (- i
                                                                                            _var40)))
                                                                                (let ((_var43 1))
                                                                                     (let ((_var44 (- j
                                                                                                      _var43)))
                                                                                          (let ((_var39 (swapij _var38
                                                                                                                _var41
                                                                                                                _var44
                                                                                                                ival
                                                                                                                jval)))
                                                                                               (let ((_var46 (cons ival
                                                                                                                   _var39)))
                                                                                                    _var46)))))))
                                                                 (let ((_var47 (car list)))
                                                                      (let ((_var49 (cdr list)))
                                                                           (let ((_var51 1))
                                                                                (let ((_var52 (- i
                                                                                                 _var51)))
                                                                                     (let ((_var54 1))
                                                                                          (let ((_var55 (- j
                                                                                                           _var54)))
                                                                                               (let ((_var50 (swapij _var49
                                                                                                                     _var52
                                                                                                                     _var55
                                                                                                                     ival
                                                                                                                     jval)))
                                                                                                    (let ((_var48 (cons _var47
                                                                                                                        _var50)))
                                                                                                         _var48)))))))))))))))))))))

(define (swap list i j)
        (let ((ival (getVal list
                            i)))
             (let ((jval (getVal list
                                 j)))
                  (let ((_var58 (swapij list
                                        i
                                        j
                                        ival
                                        jval)))
                       _var58))))

(define (loop list i k n result)
        (let ((_var59 (eq? i
                           n)))
             (if _var59
                 result
                 (let ((_var60 1))
                      (let ((_var61 (- n
                                       _var60)))
                           (let ((newlist (swap list
                                                i
                                                _var61)))
                                (let ((_var63 1))
                                     (let ((_var64 (- n
                                                      _var63)))
                                          (let ((cresult (permute newlist
                                                                  k
                                                                  _var64)))
                                               (let ((_var66 1))
                                                    (let ((_var67 (+ i
                                                                     _var66)))
                                                         (let ((_var69 (append result
                                                                               cresult)))
                                                              (let ((_var68 (loop list
                                                                                  _var67
                                                                                  k
                                                                                  n
                                                                                  _var69)))
                                                                   _var68)))))))))))))

(define (permute list k n)
        (let ((_var71 (eq? k
                           n)))
             (if _var71
                 (let ((_var72 '()))
                      (let ((_var73 (cons list
                                          _var72)))
                           _var73))
                 (let ((_var74 '()))
                      (let ((_var75 (loop list
                                          k
                                          k
                                          n
                                          _var74)))
                           _var75)))))

(define (makeList n list)
        (let ((_var76 0))
             (let ((_var77 (eq? _var76
                                n)))
                  (if _var77
                      list
                      (let ((_var78 1))
                           (let ((_var79 (- n
                                            _var78)))
                                (let ((_var81 (cons n
                                                    list)))
                                     (let ((_var80 (makeList _var79
                                                             _var81)))
                                          _var80))))))))

(define (permutation n)
        (let ((_var83 '()))
             (let ((_var84 (makeList n
                                     _var83)))
                  (let ((_var86 0))
                       (let ((_var85 (permute _var84
                                              _var86
                                              n)))
                            _var85)))))

(define (make-queue size)
        (let ((_var88 0))
             (let ((_var89 (eq? size
                                _var88)))
                  (if _var89
                      (let ((_var90 '()))
                           _var90)
                      (let ((_var91 '()))
                           (let ((_var93 1))
                                (let ((_var94 (- size
                                                 _var93)))
                                     (let ((_var95 (make-queue _var94)))
                                          (let ((_var92 (cons _var91
                                                              _var95)))
                                               _var92)))))))))

(define (fill i n queue)
        (let ((_var97 0))
             (let ((_var98 (eq? _var97
                                i)))
                  (if _var98
                      (let ((_var99 (permutation n)))
                           (let ((_var101 (cdr queue)))
                                (let ((_var100 (cons _var99
                                                     _var101)))
                                     _var100)))
                      (let ((_var103 (car queue)))
                           (let ((_var105 1))
                                (let ((_var106 (- i
                                                  _var105)))
                                     (let ((_var108 (cdr queue)))
                                          (let ((_var107 (fill _var106
                                                               n
                                                               _var108)))
                                               (let ((_var104 (cons _var103
                                                                    _var107)))
                                                    _var104))))))))))

(define (fill-queue i j n queue)
        (let ((_var111 (< i
                          j)))
             (if _var111
                 (let ((newq (fill i
                                   n
                                   queue)))
                      (let ((_var112 1))
                           (let ((_var113 (+ i
                                             _var112)))
                                (let ((_var114 (fill-queue _var113
                                                           j
                                                           n
                                                           newq)))
                                     _var114))))
                 queue)))

(define (get-ref j queue)
        (let ((_var115 0))
             (let ((_var116 (eq? _var115
                                 j)))
                  (if _var116
                      (let ((_var117 (car queue)))
                           _var117)
                      (let ((_var118 1))
                           (let ((_var119 (- j
                                             _var118)))
                                (let ((_var121 (cdr queue)))
                                     (let ((_var120 (get-ref _var119
                                                             _var121)))
                                          _var120))))))))

(define (set-ref i ref queue)
        (let ((_var123 0))
             (let ((_var124 (eq? _var123
                                 i)))
                  (if _var124
                      (let ((_var125 (cdr queue)))
                           (let ((_var126 (cons ref
                                                _var125)))
                                _var126))
                      (let ((_var127 (car queue)))
                           (let ((_var129 1))
                                (let ((_var130 (- i
                                                  _var129)))
                                     (let ((_var132 (cdr queue)))
                                          (let ((_var131 (set-ref _var130
                                                                  ref
                                                                  _var132)))
                                               (let ((_var128 (cons _var127
                                                                    _var131)))
                                                    _var128))))))))))

(define (loop-flush i k l queue)
        (let ((_var135 (< i
                          k)))
             (if _var135
                 (let ((j (+ i
                             l)))
                      (let ((_var136 (< j
                                        k)))
                           (if _var136
                               (let ((ref (get-ref j
                                                   queue)))
                                    (let ((newq (set-ref i
                                                         ref
                                                         queue)))
                                         (let ((_var137 1))
                                              (let ((_var138 (+ _var137
                                                                i)))
                                                   (let ((_var139 (loop-flush _var138
                                                                              k
                                                                              l
                                                                              newq)))
                                                        _var139)))))
                               (let ((_var140 '()))
                                    (let ((newq (set-ref i
                                                         _var140
                                                         queue)))
                                         (let ((_var142 1))
                                              (let ((_var143 (+ _var142
                                                                i)))
                                                   (let ((_var144 (loop-flush _var143
                                                                              k
                                                                              l
                                                                              newq)))
                                                        _var144))))))))
                 queue)))

(define (flush-queue k l queue)
        (let ((_var145 0))
             (let ((_var146 (loop-flush _var145
                                        k
                                        l
                                        queue)))
                  _var146)))

(define (run-operations m n k l queue)
        (let ((_var147 0))
             (let ((_var148 (eq? _var147
                                 m)))
                  (if _var148
                      queue
                      (let ((_var149 (- k
                                        l)))
                           (let ((queuefill (fill-queue _var149
                                                        k
                                                        n
                                                        queue)))
                                (let ((queueflush (flush-queue k
                                                               l
                                                               queuefill)))
                                     (let ((_var151 1))
                                          (let ((_var152 (- m
                                                            _var151)))
                                               (let ((_var153 (run-operations _var152
                                                                              n
                                                                              k
                                                                              l
                                                                              queueflush)))
                                                    _var153))))))))))

(define (MpermNKL-benchmark m n k l)
        (let ((queue (make-queue k)))
             (let ((_var154 (run-operations m
                                            n
                                            k
                                            l
                                            queue)))
                  _var154)))

(define (tenperm-benchmark m n k l)
        (let ((_var155 (MpermNKL-benchmark m
                                           n
                                           k
                                           l)))
             _var155))



(define (length l)
  (let ((c (null? l)))
    (if c
	(let ((res 0))
	  res)
	(let ((k 1))
	  (let ((rest (cdr l)))
	    (let ((tail-len (length rest)))
	      (let ((len (+ k tail-len)))
		len)))))))

(let ((_var156 1000))
     (let ((_var158 5))
          (let ((_var160 20))
               (let ((_var162 2))
                    (let ((_var157 (tenperm-benchmark _var156
                                                      _var158
                                                      _var160
                                                      _var162)))
		      (let ((_var163 (car _var157)))
                        (let ((_var164 (length _var163)))
                         _var157)))))))