(define (list listx)
        (let ((_var0 '()))
             (let ((_var1 (cons listx
                                _var0)))
                  _var1)))

(define (id idx)
        idx)

(define (firstcol fcx)
        (let ((_var2 (null? fcx)))
             (if _var2
                 (let ((_var3 '()))
                      _var3)
                 (let ((_var4 (car fcx)))
                      (let ((_var5 (car _var4)))
                           (let ((_var7 (cdr fcx)))
                                (let ((_var8 (firstcol _var7)))
                                     (let ((_var6 (cons _var5
                                                        _var8)))
                                          _var6))))))))

(define (rest rx)
        (let ((_var10 (null? rx)))
             (if _var10
                 (let ((_var11 '()))
                      _var11)
                 (let ((_var12 (car rx)))
                      (let ((_var13 (cdr _var12)))
                           (let ((_var15 (cdr rx)))
                                (let ((_var16 (rest _var15)))
                                     (let ((_var14 (cons _var13
                                                         _var16)))
                                          _var14))))))))

(define (transpose tx)
        (let ((_var18 (car tx)))
             (let ((_var19 (null? _var18)))
                  (if _var19
                      (let ((_var20 '()))
                           _var20)
                      (let ((_var21 (firstcol tx)))
                           (let ((_var23 (rest tx)))
                                (let ((_var24 (transpose _var23)))
                                     (let ((_var22 (cons _var21
                                                         _var24)))
                                          _var22))))))))

(define (append ax ay)
        (let ((_var26 (null? ax)))
             (if _var26
                 ay
                 (let ((_var27 (car ax)))
                      (let ((_var29 (cdr ax)))
                           (let ((_var30 (append _var29
                                                 ay)))
                                (let ((_var28 (cons _var27
                                                    _var30)))
                                     _var28)))))))

(define (concat cx)
        (let ((_var32 (null? cx)))
             (if _var32
                 (let ((_var33 '()))
                      _var33)
                 (let ((_var34 (car cx)))
                      (let ((_var36 (cdr cx)))
                           (let ((_var37 (concat _var36)))
                                (let ((_var35 (append _var34
                                                      _var37)))
                                     _var35)))))))

(define (length lx)
        (let ((_var39 (null? lx)))
             (if _var39
                 (let ((_var40 0))
                      _var40)
                 (let ((_var41 1))
                      (let ((_var43 (cdr lx)))
                           (let ((_var44 (length _var43)))
                                (let ((_var42 (+ _var41
                                                 _var44)))
                                     _var42)))))))

(define (take tkn tkx)
        (let ((_var46 0))
             (let ((_var47 (eq? tkn
                                _var46)))
                  (if _var47
                      (let ((_var48 '()))
                           _var48)
                      (let ((_var49 (null? tkx)))
                           (if _var49
                               (let ((_var50 '()))
                                    _var50)
                               (let ((_var51 (car tkx)))
                                    (let ((_var53 1))
                                         (let ((_var54 (- tkn
                                                          _var53)))
                                              (let ((_var56 (cdr tkx)))
                                                   (let ((_var55 (take _var54
                                                                       _var56)))
                                                        (let ((_var52 (cons _var51
                                                                            _var55)))
                                                             _var52))))))))))))

(define (drop dn dx)
        (let ((_var59 0))
             (let ((_var60 (eq? dn
                                _var59)))
                  (if _var60
                      dx
                      (let ((_var61 (null? dx)))
                           (if _var61
                               (let ((_var62 '()))
                                    _var62)
                               (let ((_var63 1))
                                    (let ((_var64 (- dn
                                                     _var63)))
                                         (let ((_var66 (cdr dx)))
                                              (let ((_var65 (drop _var64
                                                                  _var66)))
                                                   _var65))))))))))

(define (and anx any)
        (let ((_var68 #t))
             (let ((_var69 (eq? anx
                                _var68)))
                  (if _var69
                      any
                      (let ((_var70 #f))
                           _var70)))))

(define (or orx ory)
        (let ((_var71 #t))
             (let ((_var72 (eq? orx
                                _var71)))
                  (if _var72
                      orx
                      ory))))

(define (not ntx)
        (if ntx
            (let ((_var73 #f))
                 _var73)
            (let ((_var74 #t))
                 _var74)))

(define (empty ex)
        (let ((_var75 0))
             (let ((_var76 (eq? _var75
                                ex)))
                  _var76)))

(define (single sx)
        (let ((_var77 (length sx)))
             (let ((_var79 1))
                  (let ((_var78 (eq? _var77
                                     _var79)))
                       (if _var78
                           (let ((_var81 #t))
                                _var81)
                           (let ((_var82 #f))
                                _var82))))))

(define (map-cols mcx)
        (let ((_var83 (null? mcx)))
             (if _var83
                 (let ((_var84 '()))
                      _var84)
                 (let ((_var85 (car mcx)))
                      (let ((_var86 (cols _var85)))
                           (let ((_var88 (cdr mcx)))
                                (let ((_var89 (map-cols _var88)))
                                     (let ((_var87 (cons _var86
                                                         _var89)))
                                          _var87))))))))

(define (rows rsx)
        (let ((_var91 (id rsx)))
             _var91))

(define (cols csx)
        (let ((_var92 (transpose csx)))
             _var92))

(define (boxes bsx)
        (let ((_var93 (pack bsx)))
             (let ((_var94 (map-cols _var93)))
                  (let ((_var95 (unpack _var94)))
                       _var95))))

(define (map-split msx)
        (let ((_var96 (null? msx)))
             (if _var96
                 (let ((_var97 '()))
                      _var97)
                 (let ((_var98 (car msx)))
                      (let ((_var99 (split _var98)))
                           (let ((_var101 (cdr msx)))
                                (let ((_var102 (map-split _var101)))
                                     (let ((_var100 (cons _var99
                                                          _var102)))
                                          _var100))))))))

(define (map-concat mcox)
        (let ((_var104 (null? mcox)))
             (if _var104
                 (let ((_var105 '()))
                      _var105)
                 (let ((_var106 (car mcox)))
                      (let ((_var107 (concat _var106)))
                           (let ((_var109 (cdr mcox)))
                                (let ((_var110 (map-concat _var109)))
                                     (let ((_var108 (cons _var107
                                                          _var110)))
                                          _var108))))))))

(define (unpack ux)
        (let ((_var112 (concat ux)))
             (let ((_var113 (map-concat _var112)))
                  _var113)))

(define (chop chn chx)
        (let ((_var114 (null? chx)))
             (if _var114
                 chx
                 (let ((_var115 (take chn
                                      chx)))
                      (let ((_var117 (drop chn
                                           chx)))
                           (let ((_var118 (chop chn
                                                _var117)))
                                (let ((_var116 (cons _var115
                                                     _var118)))
                                     _var116)))))))

(define (pack pax)
        (let ((_var120 (map-split pax)))
             (let ((_var121 (split _var120)))
                  _var121)))

(define (elem elv elx)
        (let ((_var122 (null? elx)))
             (if _var122
                 (let ((_var123 #f))
                      _var123)
                 (let ((_var124 (car elx)))
                      (let ((_var125 (eq? _var124
                                          elv)))
                           (if _var125
                               (let ((_var126 #t))
                                    _var126)
                               (let ((_var127 (cdr elx)))
                                    (let ((_var128 (elem elv
                                                         _var127)))
                                         _var128))))))))

(define (alltrue atx)
        (let ((_var129 (null? atx)))
             (if _var129
                 (let ((_var130 #t))
                      _var130)
                 (let ((_var131 (car atx)))
                      (let ((_var133 #f))
                           (let ((_var132 (eq? _var131
                                               _var133)))
                                (if _var132
                                    (let ((_var135 #f))
                                         _var135)
                                    (let ((_var136 (cdr atx)))
                                         (let ((_var137 (alltrue _var136)))
                                              _var137)))))))))

(define (nodups ndx)
        (let ((_var138 (null? ndx)))
             (if _var138
                 (let ((_var139 #t))
                      _var139)
                 (let ((_var140 (car ndx)))
                      (let ((_var142 (cdr ndx)))
                           (let ((_var141 (elem _var140
                                                _var142)))
                                (let ((_var144 (not _var141)))
                                     (let ((_var146 (cdr ndx)))
                                          (let ((_var147 (nodups _var146)))
                                               (let ((_var145 (and _var144
                                                                   _var147)))
                                                    _var145))))))))))

(define (map-nodups mndx)
        (let ((_var149 (null? mndx)))
             (if _var149
                 (let ((_var150 '()))
                      _var150)
                 (let ((_var151 (car mndx)))
                      (let ((_var152 (nodups _var151)))
                           (let ((_var154 (cdr mndx)))
                                (let ((_var155 (map-nodups _var154)))
                                     (let ((_var153 (cons _var152
                                                          _var155)))
                                          _var153))))))))

(define (map-choice mchx)
        (let ((_var157 (null? mchx)))
             (if _var157
                 (let ((_var158 '()))
                      _var158)
                 (let ((_var159 (car mchx)))
                      (let ((_var160 (choice _var159)))
                           (let ((_var162 (cdr mchx)))
                                (let ((_var163 (map-choice _var162)))
                                     (let ((_var161 (cons _var160
                                                          _var163)))
                                          _var161))))))))

(define (map-map-choice mmchx)
        (let ((_var165 (null? mmchx)))
             (if _var165
                 (let ((_var166 '()))
                      _var166)
                 (let ((_var167 (car mmchx)))
                      (let ((_var168 (map-choice _var167)))
                           (let ((_var170 (cdr mmchx)))
                                (let ((_var171 (map-map-choice _var170)))
                                     (let ((_var169 (cons _var168
                                                          _var171)))
                                          _var169))))))))

(define (choices chsx)
        (let ((_var173 (map-map-choice chsx)))
             _var173))

(define (map-cp mcpx)
        (let ((_var174 (null? mcpx)))
             (if _var174
                 (let ((_var175 '()))
                      _var175)
                 (let ((_var176 (car mcpx)))
                      (let ((_var177 (cp _var176)))
                           (let ((_var179 (cdr mcpx)))
                                (let ((_var180 (map-cp _var179)))
                                     (let ((_var178 (cons _var177
                                                          _var180)))
                                          _var178))))))))

(define (cp cpx)
        (let ((_var182 (null? cpx)))
             (if _var182
                 (let ((_var183 '()))
                      (let ((_var184 (list _var183)))
                           _var184))
                 (let ((_var185 (car cpx)))
                      (let ((_var187 (cdr cpx)))
                           (let ((_var188 (cp _var187)))
                                (let ((_var186 (cphelp _var185
                                                       _var188)))
                                     (let ((_var190 (concat _var186)))
                                          _var190))))))))

(define (cphelp cphx cphy)
        (let ((_var191 (null? cphx)))
             (if _var191
                 (let ((_var192 '()))
                      _var192)
                 (let ((_var193 (car cphx)))
                      (let ((_var194 (cphelp2 _var193
                                              cphy)))
                           (let ((_var196 (cdr cphx)))
                                (let ((_var197 (cphelp _var196
                                                       cphy)))
                                     (let ((_var195 (cons _var194
                                                          _var197)))
                                          _var195))))))))

(define (cphelp2 cph2x cph2y)
        (let ((_var199 (null? cph2y)))
             (if _var199
                 (let ((_var200 '()))
                      _var200)
                 (let ((_var201 (car cph2y)))
                      (let ((_var202 (cons cph2x
                                           _var201)))
                           (let ((_var204 (cdr cph2y)))
                                (let ((_var205 (cphelp2 cph2x
                                                        _var204)))
                                     (let ((_var203 (cons _var202
                                                          _var205)))
                                          _var203))))))))

(define (collapse cex)
        (let ((_var207 (map-cp cex)))
             (let ((_var208 (cp _var207)))
                  _var208)))

(define (minus mx my)
        (let ((_var209 (single mx)))
             (if _var209
                 mx
                 (let ((_var210 (minushelp mx
                                           my)))
                      _var210))))

(define (minushelp mshx mshy)
        (let ((_var211 (null? mshy)))
             (if _var211
                 mshx
                 (let ((_var212 (car mshy)))
                      (let ((_var213 (remove _var212
                                             mshx)))
                           (let ((_var215 (cdr mshy)))
                                (let ((_var214 (minushelp _var213
                                                          _var215)))
                                     _var214)))))))

(define (remove rmvv rmvx)
        (let ((_var217 (null? rmvx)))
             (if _var217
                 rmvx
                 (let ((_var218 (car rmvx)))
                      (let ((_var219 (eq? _var218
                                          rmvv)))
                           (if _var219
                               (let ((_var220 (cdr rmvx)))
                                    (let ((_var221 (remove rmvv
                                                           _var220)))
                                         _var221))
                               (let ((_var222 (car rmvx)))
                                    (let ((_var224 (cdr rmvx)))
                                         (let ((_var225 (remove rmvv
                                                                _var224)))
                                              (let ((_var223 (cons _var222
                                                                   _var225)))
                                                   _var223))))))))))

(define (filter-single fslx)
        (let ((_var227 (null? fslx)))
             (if _var227
                 (let ((_var228 '()))
                      _var228)
                 (let ((_var229 (car fslx)))
                      (let ((_var230 (single _var229)))
                           (if _var230
                               (let ((_var231 (car fslx)))
                                    (let ((_var233 (cdr fslx)))
                                         (let ((_var234 (filter-single _var233)))
                                              (let ((_var232 (cons _var231
                                                                   _var234)))
                                                   _var232))))
                               (let ((_var236 (cdr fslx)))
                                    (let ((_var237 (filter-single _var236)))
                                         _var237))))))))

(define (reduce redx)
        (let ((_var238 (filter-single redx)))
             (let ((singles (concat _var238)))
                  (let ((_var240 (reducehelp redx
                                             singles)))
                       _var240))))

(define (reducehelp redhx rsingles)
        (let ((_var241 (null? redhx)))
             (if _var241
                 (let ((_var242 '()))
                      _var242)
                 (let ((_var243 (car redhx)))
                      (let ((_var244 (minus _var243
                                            rsingles)))
                           (let ((_var246 (cdr redhx)))
                                (let ((_var247 (reducehelp _var246
                                                           rsingles)))
                                     (let ((_var245 (cons _var244
                                                          _var247)))
                                          _var245))))))))

(define (prune prux)
        (let ((_var249 (pruneby-rows prux)))
             (let ((_var250 (pruneby-cols _var249)))
                  (let ((_var251 (pruneby-boxes _var250)))
                       _var251))))

(define (map-reduce mredx)
        (let ((_var252 (null? mredx)))
             (if _var252
                 (let ((_var253 '()))
                      _var253)
                 (let ((_var254 (car mredx)))
                      (let ((_var255 (reduce _var254)))
                           (let ((_var257 (cdr mredx)))
                                (let ((_var258 (map-reduce _var257)))
                                     (let ((_var256 (cons _var255
                                                          _var258)))
                                          _var256))))))))

(define (pruneby-boxes prubx)
        (let ((_var260 (boxes prubx)))
             (let ((_var261 (map-reduce _var260)))
                  (let ((_var262 (boxes _var261)))
                       _var262))))

(define (pruneby-cols prucx)
        (let ((_var263 (cols prucx)))
             (let ((_var264 (map-reduce _var263)))
                  (let ((_var265 (cols _var264)))
                       _var265))))

(define (pruneby-rows prurx)
        (let ((_var266 (rows prurx)))
             (let ((_var267 (map-reduce _var266)))
                  (let ((_var268 (rows _var267)))
                       _var268))))

(define (all-single asex)
        (let ((_var269 (map-single asex)))
             (let ((_var270 (alltrue _var269)))
                  _var270)))

(define (map-single msex)
        (let ((_var271 (null? msex)))
             (if _var271
                 (let ((_var272 '()))
                      _var272)
                 (let ((_var273 (car msex)))
                      (let ((_var274 (single _var273)))
                           (let ((_var276 (cdr msex)))
                                (let ((_var277 (map-single _var276)))
                                     (let ((_var275 (cons _var274
                                                          _var277)))
                                          _var275))))))))

(define (all-all-single aasex)
        (let ((_var279 (map-all-single aasex)))
             (let ((_var280 (alltrue _var279)))
                  _var280)))

(define (map-all-single masex)
        (let ((_var281 (null? masex)))
             (if _var281
                 (let ((_var282 '()))
                      _var282)
                 (let ((_var283 (car masex)))
                      (let ((_var284 (all-single _var283)))
                           (let ((_var286 (cdr masex)))
                                (let ((_var287 (map-all-single _var286)))
                                     (let ((_var285 (cons _var284
                                                          _var287)))
                                          _var285))))))))

(define (complete cmex)
        (let ((_var289 (all-all-single cmex)))
             _var289))

(define (any-null anlx)
        (let ((_var290 (null? anlx)))
             (if _var290
                 (let ((_var291 #f))
                      _var291)
                 (let ((_var292 (car anlx)))
                      (let ((_var293 (null? _var292)))
                           (if _var293
                               (let ((_var294 #t))
                                    _var294)
                               (let ((_var295 (cdr anlx)))
                                    (let ((_var296 (any-null _var295)))
                                         _var296))))))))

(define (any-any-null aanlx)
        (let ((_var297 (null? aanlx)))
             (if _var297
                 (let ((_var298 #f))
                      _var298)
                 (let ((_var299 (car aanlx)))
                      (let ((_var300 (any-null _var299)))
                           (if _var300
                               (let ((_var301 #t))
                                    _var301)
                               (let ((_var302 (cdr aanlx)))
                                    (let ((_var303 (any-any-null _var302)))
                                         _var303))))))))

(define (void vx)
        (let ((_var304 (any-any-null vx)))
             _var304))

(define (all-consistent actx)
        (let ((_var305 (map-consistent actx)))
             (let ((_var306 (alltrue _var305)))
                  _var306)))

(define (map-consistent mctx)
        (let ((_var307 (null? mctx)))
             (if _var307
                 (let ((_var308 '()))
                      _var308)
                 (let ((_var309 (car mctx)))
                      (let ((_var310 (consistent _var309)))
                           (let ((_var312 (cdr mctx)))
                                (let ((_var313 (map-consistent _var312)))
                                     (let ((_var311 (cons _var310
                                                          _var313)))
                                          _var311))))))))

(define (consistent ctx)
        (let ((_var315 (filter-single ctx)))
             (let ((_var316 (concat _var315)))
                  (let ((_var317 (nodups _var316)))
                       _var317))))

(define (safe sfex)
        (let ((_var318 (rows sfex)))
             (let ((_var319 (all-consistent _var318)))
                  (let ((_var321 (cols sfex)))
                       (let ((_var322 (all-consistent _var321)))
                            (let ((_var324 (boxes sfex)))
                                 (let ((_var325 (all-consistent _var324)))
                                      (let ((_var323 (and _var322
                                                          _var325)))
                                           (let ((_var320 (and _var319
                                                               _var323)))
                                                _var320)))))))))

(define (blocked bkdx)
        (let ((_var328 (null? bkdx)))
             (let ((_var330 (void bkdx)))
                  (let ((_var332 (safe bkdx)))
                       (let ((_var333 (not _var332)))
                            (let ((_var331 (or _var330
                                               _var333)))
                                 (let ((_var329 (or _var328
                                                    _var331)))
                                      _var329)))))))

(define (search m)
        (let ((_var336 (blocked m)))
             (if _var336
                 (let ((_var337 '()))
                      _var337)
                 (let ((_var338 (complete m)))
                      (if _var338
                          (let ((_var339 (collapse m)))
                               _var339)
                          (let ((m1 (expand m)))
                               (let ((_var340 (searchhelp m1)))
                                    _var340)))))))

(define (searchhelp m2)
        (let ((_var341 (null? m2)))
             (if _var341
                 (let ((_var342 '()))
                      _var342)
                 (let ((_var343 (car m2)))
                      (let ((_var344 (prune _var343)))
                           (let ((s2 (search _var344)))
                                (let ((_var346 (single s2)))
                                     (if _var346
                                         s2
                                         (let ((_var347 (cdr m2)))
                                              (let ((_var348 (searchhelp _var347)))
                                                   _var348))))))))))

(define (not-single nsx)
        (let ((_var349 (single nsx)))
             (let ((_var350 (not _var349)))
                  _var350)))

(define (any-not-single ansx)
        (let ((_var351 (null? ansx)))
             (if _var351
                 (let ((_var352 #f))
                      _var352)
                 (let ((_var353 (car ansx)))
                      (let ((_var354 (not-single _var353)))
                           (if _var354
                               (let ((_var355 #t))
                                    _var355)
                               (let ((_var356 (cdr ansx)))
                                    (let ((_var357 (any-not-single _var356)))
                                         _var357))))))))

(define (until-any-not-single uansx)
        (let ((_var358 (null? uansx)))
             (if _var358
                 (let ((_var359 '()))
                      _var359)
                 (let ((_var360 (car uansx)))
                      (let ((_var361 (any-not-single _var360)))
                           (if _var361
                               (let ((_var362 '()))
                                    _var362)
                               (let ((_var363 (car uansx)))
                                    (let ((_var365 (cdr uansx)))
                                         (let ((_var366 (until-any-not-single _var365)))
                                              (let ((_var364 (cons _var363
                                                                   _var366)))
                                                   _var364))))))))))

(define (after-any-not-single aansx)
        (let ((_var368 (null? aansx)))
             (if _var368
                 (let ((_var369 '()))
                      _var369)
                 (let ((_var370 (car aansx)))
                      (let ((_var371 (any-not-single _var370)))
                           (if _var371
                               aansx
                               (let ((_var372 (cdr aansx)))
                                    (let ((_var373 (after-any-not-single _var372)))
                                         _var373))))))))

(define (until-not-single unsx)
        (let ((_var374 (null? unsx)))
             (if _var374
                 (let ((_var375 '()))
                      _var375)
                 (let ((_var376 (car unsx)))
                      (let ((_var377 (not-single _var376)))
                           (if _var377
                               (let ((_var378 '()))
                                    _var378)
                               (let ((_var379 (car unsx)))
                                    (let ((_var381 (cdr unsx)))
                                         (let ((_var382 (until-not-single _var381)))
                                              (let ((_var380 (cons _var379
                                                                   _var382)))
                                                   _var380))))))))))

(define (after-not-single afnsx)
        (let ((_var384 (null? afnsx)))
             (if _var384
                 (let ((_var385 '()))
                      _var385)
                 (let ((_var386 (car afnsx)))
                      (let ((_var387 (not-single _var386)))
                           (if _var387
                               afnsx
                               (let ((_var388 (cdr afnsx)))
                                    (let ((_var389 (after-not-single _var388)))
                                         _var389))))))))

(define (expand em)
        (let ((vrows1 (until-any-not-single em)))
             (let ((vrows2tmp (after-any-not-single em)))
                  (let ((vrow (car vrows2tmp)))
                       (let ((vrows2 (cdr vrows2tmp)))
                            (let ((vrow1 (until-not-single vrow)))
                                 (let ((vrow2tmp (after-not-single vrow)))
                                      (let ((vcs (car vrow2tmp)))
                                           (let ((vrow2 (cdr vrow2tmp)))
                                                (let ((_var390 (expandhelp vrows1
                                                                           vrow1
                                                                           vrow2
                                                                           vrows2
                                                                           vcs)))
                                                     _var390))))))))))

(define (expandhelp vrs1 vr1 vr2 vrs2 ehx)
        (let ((_var391 (null? ehx)))
             (if _var391
                 (let ((_var392 '()))
                      _var392)
                 (let ((_var393 (car ehx)))
                      (let ((_var394 (list _var393)))
                           (let ((_var395 (cons _var394
                                                vr2)))
                                (let ((vnr (append vr1
                                                   _var395)))
                                     (let ((_var397 (cons vnr
                                                          vrs2)))
                                          (let ((vnb (append vrs1
                                                             _var397)))
                                               (let ((_var399 (cdr ehx)))
                                                    (let ((_var400 (expandhelp vrs1
                                                                               vr1
                                                                               vr2
                                                                               vrs2
                                                                               _var399)))
                                                         (let ((_var401 (cons vnb
                                                                              _var400)))
                                                              _var401))))))))))))

(define (solve slvx)
        (let ((_var402 (choices slvx)))
             (let ((_var403 (prune _var402)))
                  (let ((_var404 (search _var403)))
                       _var404))))

(define (split spx)
        (let ((boxsize 3))
             (let ((_var405 (chop boxsize
                                  spx)))
                  _var405)))

(define (choice chv)
        (let ((_var406 1))
             (let ((_var408 2))
                  (let ((_var410 3))
                       (let ((_var412 4))
                            (let ((_var414 5))
                                 (let ((_var416 6))
                                      (let ((_var418 7))
                                           (let ((_var420 8))
                                                (let ((_var422 9))
                                                     (let ((_var424 '()))
                                                          (let ((_var423 (cons _var422
                                                                               _var424)))
                                                               (let ((_var421 (cons _var420
                                                                                    _var423)))
                                                                    (let ((_var419 (cons _var418
                                                                                         _var421)))
                                                                         (let ((_var417 (cons _var416
                                                                                              _var419)))
                                                                              (let ((_var415 (cons _var414
                                                                                                   _var417)))
                                                                                   (let ((_var413 (cons _var412
                                                                                                        _var415)))
                                                                                        (let ((_var411 (cons _var410
                                                                                                             _var413)))
                                                                                             (let ((_var409 (cons _var408
                                                                                                                  _var411)))
                                                                                                  (let ((vals (cons _var406
                                                                                                                    _var409)))
                                                                                                       (let ((_var434 (empty chv)))
                                                                                                            (if _var434
                                                                                                                vals
                                                                                                                (let ((_var435 (list chv)))
                                                                                                                     _var435)))))))))))))))))))))))

(define (solve-in-loop b)
        (let ((_var0 2))
             (let ((_var1 (solve-in-loop-helper b
                                                _var0)))
                  _var1)))

(define (solve-in-loop-helper b n)
        (let ((_var2 0))
             (let ((_var3 (= n
                             _var2)))
                  (if _var3
                      (let ((_var4 '()))
                           _var4)
                      (let ((_var5 (solve b)))
                           (let ((_var7 1))
                                (let ((_var8 (- n
                                                _var7)))
                                     (let ((_var9 (solve-in-loop-helper b
                                                                        _var8)))
                                          (let ((_var6 (cons _var5
                                                             _var9)))
                                               _var6)))))))))

(let ((_var436 6))
     (let ((_var438 0))
          (let ((_var440 0))
               (let ((_var442 1))
                    (let ((_var444 0))
                         (let ((_var446 8))
                              (let ((_var448 2))
                                   (let ((_var450 0))
                                        (let ((_var452 3))
                                             (let ((_var454 '()))
                                                  (let ((_var453 (cons _var452
                                                                       _var454)))
                                                       (let ((_var451 (cons _var450
                                                                            _var453)))
                                                            (let ((_var449 (cons _var448
                                                                                 _var451)))
                                                                 (let ((_var447 (cons _var446
                                                                                      _var449)))
                                                                      (let ((_var445 (cons _var444
                                                                                           _var447)))
                                                                           (let ((_var443 (cons _var442
                                                                                                _var445)))
                                                                                (let ((_var441 (cons _var440
                                                                                                     _var443)))
                                                                                     (let ((_var439 (cons _var438
                                                                                                          _var441)))
                                                                                          (let ((_var437 (cons _var436
                                                                                                               _var439)))
                                                                                               (let ((_var465 0))
                                                                                                    (let ((_var467 2))
                                                                                                         (let ((_var469 0))
                                                                                                              (let ((_var471 0))
                                                                                                                   (let ((_var473 4))
                                                                                                                        (let ((_var475 0))
                                                                                                                             (let ((_var477 0))
                                                                                                                                  (let ((_var479 9))
                                                                                                                                       (let ((_var481 0))
                                                                                                                                            (let ((_var483 '()))
                                                                                                                                                 (let ((_var482 (cons _var481
                                                                                                                                                                      _var483)))
                                                                                                                                                      (let ((_var480 (cons _var479
                                                                                                                                                                           _var482)))
                                                                                                                                                           (let ((_var478 (cons _var477
                                                                                                                                                                                _var480)))
                                                                                                                                                                (let ((_var476 (cons _var475
                                                                                                                                                                                     _var478)))
                                                                                                                                                                     (let ((_var474 (cons _var473
                                                                                                                                                                                          _var476)))
                                                                                                                                                                          (let ((_var472 (cons _var471
                                                                                                                                                                                               _var474)))
                                                                                                                                                                               (let ((_var470 (cons _var469
                                                                                                                                                                                                    _var472)))
                                                                                                                                                                                    (let ((_var468 (cons _var467
                                                                                                                                                                                                         _var470)))
                                                                                                                                                                                         (let ((_var466 (cons _var465
                                                                                                                                                                                                              _var468)))
                                                                                                                                                                                              (let ((_var494 8))
                                                                                                                                                                                                   (let ((_var496 0))
                                                                                                                                                                                                        (let ((_var498 3))
                                                                                                                                                                                                             (let ((_var500 0))
                                                                                                                                                                                                                  (let ((_var502 0))
                                                                                                                                                                                                                       (let ((_var504 5))
                                                                                                                                                                                                                            (let ((_var506 4))
                                                                                                                                                                                                                                 (let ((_var508 0))
                                                                                                                                                                                                                                      (let ((_var510 0))
                                                                                                                                                                                                                                           (let ((_var512 '()))
                                                                                                                                                                                                                                                (let ((_var511 (cons _var510
                                                                                                                                                                                                                                                                     _var512)))
                                                                                                                                                                                                                                                     (let ((_var509 (cons _var508
                                                                                                                                                                                                                                                                          _var511)))
                                                                                                                                                                                                                                                          (let ((_var507 (cons _var506
                                                                                                                                                                                                                                                                               _var509)))
                                                                                                                                                                                                                                                               (let ((_var505 (cons _var504
                                                                                                                                                                                                                                                                                    _var507)))
                                                                                                                                                                                                                                                                    (let ((_var503 (cons _var502
                                                                                                                                                                                                                                                                                         _var505)))
                                                                                                                                                                                                                                                                         (let ((_var501 (cons _var500
                                                                                                                                                                                                                                                                                              _var503)))
                                                                                                                                                                                                                                                                              (let ((_var499 (cons _var498
                                                                                                                                                                                                                                                                                                   _var501)))
                                                                                                                                                                                                                                                                                   (let ((_var497 (cons _var496
                                                                                                                                                                                                                                                                                                        _var499)))
                                                                                                                                                                                                                                                                                        (let ((_var495 (cons _var494
                                                                                                                                                                                                                                                                                                             _var497)))
                                                                                                                                                                                                                                                                                             (let ((_var523 5))
                                                                                                                                                                                                                                                                                                  (let ((_var525 0))
                                                                                                                                                                                                                                                                                                       (let ((_var527 4))
                                                                                                                                                                                                                                                                                                            (let ((_var529 6))
                                                                                                                                                                                                                                                                                                                 (let ((_var531 0))
                                                                                                                                                                                                                                                                                                                      (let ((_var533 7))
                                                                                                                                                                                                                                                                                                                           (let ((_var535 0))
                                                                                                                                                                                                                                                                                                                                (let ((_var537 0))
                                                                                                                                                                                                                                                                                                                                     (let ((_var539 9))
                                                                                                                                                                                                                                                                                                                                          (let ((_var541 '()))
                                                                                                                                                                                                                                                                                                                                               (let ((_var540 (cons _var539
                                                                                                                                                                                                                                                                                                                                                                    _var541)))
                                                                                                                                                                                                                                                                                                                                                    (let ((_var538 (cons _var537
                                                                                                                                                                                                                                                                                                                                                                         _var540)))
                                                                                                                                                                                                                                                                                                                                                         (let ((_var536 (cons _var535
                                                                                                                                                                                                                                                                                                                                                                              _var538)))
                                                                                                                                                                                                                                                                                                                                                              (let ((_var534 (cons _var533
                                                                                                                                                                                                                                                                                                                                                                                   _var536)))
                                                                                                                                                                                                                                                                                                                                                                   (let ((_var532 (cons _var531
                                                                                                                                                                                                                                                                                                                                                                                        _var534)))
                                                                                                                                                                                                                                                                                                                                                                        (let ((_var530 (cons _var529
                                                                                                                                                                                                                                                                                                                                                                                             _var532)))
                                                                                                                                                                                                                                                                                                                                                                             (let ((_var528 (cons _var527
                                                                                                                                                                                                                                                                                                                                                                                                  _var530)))
                                                                                                                                                                                                                                                                                                                                                                                  (let ((_var526 (cons _var525
                                                                                                                                                                                                                                                                                                                                                                                                       _var528)))
                                                                                                                                                                                                                                                                                                                                                                                       (let ((_var524 (cons _var523
                                                                                                                                                                                                                                                                                                                                                                                                            _var526)))
                                                                                                                                                                                                                                                                                                                                                                                            (let ((_var552 0))
                                                                                                                                                                                                                                                                                                                                                                                                 (let ((_var554 3))
                                                                                                                                                                                                                                                                                                                                                                                                      (let ((_var556 0))
                                                                                                                                                                                                                                                                                                                                                                                                           (let ((_var558 0))
                                                                                                                                                                                                                                                                                                                                                                                                                (let ((_var560 0))
                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((_var562 0))
                                                                                                                                                                                                                                                                                                                                                                                                                          (let ((_var564 0))
                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((_var566 5))
                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((_var568 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((_var570 '()))
                                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((_var569 (cons _var568
                                                                                                                                                                                                                                                                                                                                                                                                                                                                   _var570)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                   (let ((_var567 (cons _var566
                                                                                                                                                                                                                                                                                                                                                                                                                                                                        _var569)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                        (let ((_var565 (cons _var564
                                                                                                                                                                                                                                                                                                                                                                                                                                                                             _var567)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                             (let ((_var563 (cons _var562
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  _var565)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                  (let ((_var561 (cons _var560
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       _var563)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                       (let ((_var559 (cons _var558
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            _var561)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                            (let ((_var557 (cons _var556
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 _var559)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 (let ((_var555 (cons _var554
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      _var557)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      (let ((_var553 (cons _var552
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           _var555)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           (let ((_var581 7))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                (let ((_var583 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((_var585 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          (let ((_var587 8))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((_var589 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((_var591 3))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((_var593 1))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((_var595 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   (let ((_var597 2))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (let ((_var599 '()))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             (let ((_var598 (cons _var597
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  _var599)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  (let ((_var596 (cons _var595
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       _var598)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       (let ((_var594 (cons _var593
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            _var596)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            (let ((_var592 (cons _var591
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 _var594)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 (let ((_var590 (cons _var589
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      _var592)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      (let ((_var588 (cons _var587
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           _var590)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           (let ((_var586 (cons _var585
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                _var588)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                (let ((_var584 (cons _var583
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     _var586)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((_var582 (cons _var581
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          _var584)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          (let ((_var610 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((_var612 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((_var614 1))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((_var616 7))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((_var618 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   (let ((_var620 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (let ((_var622 9))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             (let ((_var624 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  (let ((_var626 6))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       (let ((_var628 '()))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            (let ((_var627 (cons _var626
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 _var628)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 (let ((_var625 (cons _var624
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      _var627)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      (let ((_var623 (cons _var622
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           _var625)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           (let ((_var621 (cons _var620
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                _var623)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                (let ((_var619 (cons _var618
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     _var621)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((_var617 (cons _var616
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          _var619)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          (let ((_var615 (cons _var614
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               _var617)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((_var613 (cons _var612
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    _var615)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((_var611 (cons _var610
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         _var613)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((_var639 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((_var641 8))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   (let ((_var643 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (let ((_var645 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             (let ((_var647 3))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  (let ((_var649 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       (let ((_var651 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            (let ((_var653 2))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 (let ((_var655 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      (let ((_var657 '()))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           (let ((_var656 (cons _var655
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                _var657)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                (let ((_var654 (cons _var653
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     _var656)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((_var652 (cons _var651
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          _var654)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          (let ((_var650 (cons _var649
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               _var652)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((_var648 (cons _var647
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    _var650)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((_var646 (cons _var645
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         _var648)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((_var644 (cons _var643
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              _var646)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((_var642 (cons _var641
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   _var644)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   (let ((_var640 (cons _var639
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        _var642)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (let ((_var668 3))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             (let ((_var670 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  (let ((_var672 2))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       (let ((_var674 9))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            (let ((_var676 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 (let ((_var678 4))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      (let ((_var680 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           (let ((_var682 0))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                (let ((_var684 5))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((_var686 '()))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          (let ((_var685 (cons _var684
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               _var686)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((_var683 (cons _var682
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    _var685)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((_var681 (cons _var680
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         _var683)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((_var679 (cons _var678
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              _var681)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((_var677 (cons _var676
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   _var679)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   (let ((_var675 (cons _var674
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        _var677)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (let ((_var673 (cons _var672
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             _var675)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             (let ((_var671 (cons _var670
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  _var673)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  (let ((_var669 (cons _var668
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       _var671)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       (let ((_var697 '()))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            (let ((_var696 (cons _var669
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 _var697)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 (let ((_var667 (cons _var640
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      _var696)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      (let ((_var638 (cons _var611
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           _var667)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           (let ((_var609 (cons _var582
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                _var638)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                (let ((_var580 (cons _var553
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     _var609)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((_var551 (cons _var524
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          _var580)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          (let ((_var522 (cons _var495
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               _var551)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((_var493 (cons _var466
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    _var522)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((b1 (cons _var437
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    _var493)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((_var11 100))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((_var707 (solve-in-loop-helper b1
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   _var11)))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   _var707)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
