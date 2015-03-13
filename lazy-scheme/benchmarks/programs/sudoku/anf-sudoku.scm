(define (and a b)
        (if a 
               (if b 
                      #t 
                      #f ) 
               #f ) )

(define (or a b)
        (if a 
               a 
               b ) )

(define (list listx)
        (cons listx  () ) )

(define (id idx)
        idx )

(define (firstcol fcx)
        (let ((v16 (null? fcx ) ))
                (if v16 
                        () 
                       (let ((v921 (car fcx ) ))
                               (let ((v20 (car v921 ) ))
                                       (let ((v0 (cdr fcx ) ))
                                               (let ((v23 (firstcol v0 ) ))
                                                       (cons v20 v23 ) ))))) ))

(define (rest rx)
        (let ((v27 (null? rx ) ))
                (if v27 
                        () 
                       (let ((v955 (car rx ) ))
                               (let ((v31 (cdr v955 ) ))
                                       (let ((v948 (cdr rx ) ))
                                               (let ((v34 (rest v948 ) ))
                                                       (cons v31 v34 ) ))))) ))

(define (transpose tx)
        (let ((v991 (car tx ) ))
                 (let ((v978 (null? v991 ) ))
                          (if v978 
                                  () 
                                 (let ((v42 (firstcol tx ) ))
                                         (let ((v982 (rest tx ) ))
                                                 (let ((v45 (transpose v982 ) ))
                                                         (cons v42 v45 ) )))) )))

(define (append ax ay)
        (let ((v49 (null? ax ) ))
                 (if v49 
                         ay 
                         (let ((v52 (car ax ) ))
                                  (let ((v1015 (cdr ax ) ))
                                           (let ((v56 (append v1015 ay ) ))
                                                    (cons v52 v56 ) )))) ))

(define (concat cx)
        (let ((v60 (null? cx ) ))
                 (if v60 
                          () 
                         (let ((v63 (car cx ) ))
                                  (let ((v1034 (cdr cx ) ))
                                           (let ((v66 (concat v1034 ) ))
                                                    (append v63 v66 ) )))) ))

(define (length lx)
        (let ((v70 (null? lx ) ))
                 (if v70 
                         0 
                         (let ((v1063 (cdr lx ) ))
                                  (let ((v75 (length v1063 ) ))
                                           (+ 1 v75 ) ))) ))

(define (take tkn tkx)
        (let ((v80 (eq? tkn 0 ) ))
                 (if v80 
                          () 
                         (let ((v83 (null? tkx ) ))
                                  (if v83 
                                           () 
                                          (let ((v86 (car tkx ) ))
                                                   (let ((v1090 (- tkn 1 ) ))
                                                            (let ((v1097 (cdr tkx ) ))
                                                                     (let ((v92 (take v1090 v1097 ) ))
                                                                              (cons v86 v92 ) ))))) )) ))

(define (drop dn dx)
        (let ((v98 (eq? dn 0 ) ))
                 (if v98 
                         dx 
                         (let ((v101 (null? dx ) ))
                                  (if v101 
                                           () 
                                          (let ((v105 (- dn 1 ) ))
                                                   (let ((v107 (cdr dx ) ))
                                                            (drop v105 v107 ) ))) )) ))

(define (and anx any)
        (let ((v113 (eq? anx #t ) ))
                 (if v113 
                         any 
                         #f ) ))

(define (or orx ory)
        (let ((v119 (eq? orx #t ) ))
                 (if v119 
                         orx 
                         ory ) ))

(define (not ntx)
        (if ntx 
                #f 
                #t ) )

(define (empty ex)
        (eq? 0 ex ) )

(define (single sx)
        (let ((v1162 (length sx ) ))
                 (let ((v1157 (eq? v1162 1 ) ))
                          (if v1157 
                                  #t 
                                  #f ) )))

(define (map-cols mcx)
        (let ((v138 (null? mcx ) ))
                 (if v138 
                          () 
                         (let ((v1194 (car mcx ) ))
                                  (let ((v142 (cols v1194 ) ))
                                           (let ((v1187 (cdr mcx ) ))
                                                    (let ((v145 (map-cols v1187 ) ))
                                                             (cons v142 v145 ) ))))) ))

(define (rows rsx)
        (id rsx ) )

(define (cols csx)
        (transpose csx ) )

(define (boxes bsx)
        (let ((v1212 (pack bsx ) ))
                 (let ((v154 (map-cols v1212 ) ))
                          (unpack v154 ) )))

(define (map-split msx)
        (let ((v157 (null? msx ) ))
                 (if v157 
                          () 
                         (let ((v1243 (car msx ) ))
                                  (let ((v161 (split v1243 ) ))
                                           (let ((v1236 (cdr msx ) ))
                                                    (let ((v164 (map-split v1236 ) ))
                                                             (cons v161 v164 ) ))))) ))

(define (map-concat mcox)
        (let ((v168 (null? mcox ) ))
                 (if v168 
                          () 
                         (let ((v1278 (car mcox ) ))
                                  (let ((v172 (concat v1278 ) ))
                                           (let ((v1271 (cdr mcox ) ))
                                                    (let ((v175 (map-concat v1271 ) ))
                                                             (cons v172 v175 ) ))))) ))

(define (unpack ux)
        (let ((v179 (concat ux ) ))
                 (map-concat v179 ) ))

(define (chop chn chx)
        (let ((v182 (null? chx ) ))
                 (if v182 
                         chx 
                         (let ((v186 (take chn chx ) ))
                                  (let ((v1306 (drop chn chx ) ))
                                           (let ((v191 (chop chn v1306 ) ))
                                                    (cons v186 v191 ) )))) ))

(define (pack pax)
        (let ((v195 (map-split pax ) ))
                 (split v195 ) ))

(define (elem elv elx)
        (let ((v198 (null? elx ) ))
                 (if v198 
                         #f 
                         (let ((v1332 (car elx ) ))
                                  (let ((v1326 (eq? v1332 elv ) ))
                                           (if v1326 
                                                   #t 
                                                   (let ((v207 (cdr elx ) ))
                                                            (elem elv v207 ) )) ))) ))

(define (alltrue atx)
        (let ((v212 (null? atx ) ))
                 (if v212 
                         #t 
                         (let ((v1362 (car atx ) ))
                                  (let ((v1356 (eq? v1362 #f ) ))
                                           (if v1356 
                                                   #f 
                                                   (let ((v220 (cdr atx ) ))
                                                            (alltrue v220 ) )) ))) ))

(define (nodups ndx)
        (let ((v225 (null? ndx ) ))
                 (if v225 
                         #t 
                         (let ((v1426 (car ndx ) ))
                                  (let ((v1437 (cdr ndx ) ))
                                           (let ((v1446 (elem v1426 v1437 ) ))
                                                    (let ((v232 (not v1446 ) ))
                                                             (let ((v1416 (cdr ndx ) ))
                                                                      (let ((v235 (nodups v1416 ) ))
                                                                               (and v232 v235 ) ))))))) ))

(define (map-nodups mndx)
        (let ((v239 (null? mndx ) ))
                 (if v239 
                          () 
                         (let ((v1492 (car mndx ) ))
                                  (let ((v243 (nodups v1492 ) ))
                                           (let ((v1485 (cdr mndx ) ))
                                                    (let ((v246 (map-nodups v1485 ) ))
                                                             (cons v243 v246 ) ))))) ))

(define (map-choice mchx)
        (let ((v250 (null? mchx ) ))
                 (if v250 
                          () 
                         (let ((v1527 (car mchx ) ))
                                  (let ((v254 (choice v1527 ) ))
                                           (let ((v1520 (cdr mchx ) ))
                                                    (let ((v257 (map-choice v1520 ) ))
                                                             (cons v254 v257 ) ))))) ))

(define (map-map-choice mmchx)
        (let ((v261 (null? mmchx ) ))
                 (if v261 
                          () 
                         (let ((v1562 (car mmchx ) ))
                                  (let ((v265 (map-choice v1562 ) ))
                                           (let ((v1555 (cdr mmchx ) ))
                                                    (let ((v268 (map-map-choice v1555 ) ))
                                                             (cons v265 v268 ) ))))) ))

(define (choices chsx)
        (map-map-choice chsx ) )

(define (map-cp mcpx)
        (let ((v274 (null? mcpx ) ))
                 (if v274 
                          () 
                         (let ((v1597 (car mcpx ) ))
                                  (let ((v278 (cp v1597 ) ))
                                           (let ((v1590 (cdr mcpx ) ))
                                                    (let ((v281 (map-cp v1590 ) ))
                                                             (cons v278 v281 ) ))))) ))

(define (cp cpx)
        (let ((v285 (null? cpx ) ))
                 (if v285 
                         (list  () ) 
                         (let ((v1631 (car cpx ) ))
                                  (let ((v1640 (cdr cpx ) ))
                                           (let ((v1647 (cp v1640 ) ))
                                                    (let ((v293 (cphelp v1631 v1647 ) ))
                                                             (concat v293 ) ))))) ))

(define (cphelp cphx cphy)
        (let ((v297 (null? cphx ) ))
                 (if v297 
                          () 
                         (let ((v1689 (car cphx ) ))
                                  (let ((v302 (cphelp2 v1689 cphy ) ))
                                           (let ((v1682 (cdr cphx ) ))
                                                    (let ((v306 (cphelp v1682 cphy ) ))
                                                             (cons v302 v306 ) ))))) ))

(define (cphelp2 cph2x cph2y)
        (let ((v310 (null? cph2y ) ))
                 (if v310 
                          () 
                         (let ((v1723 (car cph2y ) ))
                                  (let ((v315 (cons cph2x v1723 ) ))
                                           (let ((v1716 (cdr cph2y ) ))
                                                    (let ((v319 (cphelp2 cph2x v1716 ) ))
                                                             (cons v315 v319 ) ))))) ))

(define (collapse cex)
        (let ((v323 (map-cp cex ) ))
                 (cp v323 ) ))

(define (minus mx my)
        (let ((v326 (single mx ) ))
                 (if v326 
                         mx 
                         (minushelp mx my ) ) ))

(define (minushelp mshx mshy)
        (let ((v333 (null? mshy ) ))
                 (if v333 
                         mshx 
                         (let ((v1755 (car mshy ) ))
                                  (let ((v338 (remove v1755 mshx ) ))
                                           (let ((v340 (cdr mshy ) ))
                                                    (minushelp v338 v340 ) )))) ))

(define (remove rmvv rmvx)
        (let ((v344 (null? rmvx ) ))
                 (if v344 
                         rmvx 
                         (let ((v1801 (car rmvx ) ))
                                  (let ((v1787 (eq? v1801 rmvv ) ))
                                           (if v1787 
                                                   (let ((v352 (cdr rmvx ) ))
                                                            (remove rmvv v352 ) )
                                                   (let ((v355 (car rmvx ) ))
                                                            (let ((v1792 (cdr rmvx ) ))
                                                                     (let ((v359 (remove rmvv v1792 ) ))
                                                                              (cons v355 v359 ) )))) ))) ))

(define (filter-single fslx)
        (let ((v364 (null? fslx ) ))
                 (if v364 
                          () 
                         (let ((v1856 (car fslx ) ))
                                  (let ((v1842 (single v1856 ) ))
                                           (if v1842 
                                                   (let ((v370 (car fslx ) ))
                                                            (let ((v1846 (cdr fslx ) ))
                                                                     (let ((v373 (filter-single v1846 ) ))
                                                                              (cons v370 v373 ) )))
                                                   (let ((v376 (cdr fslx ) ))
                                                            (filter-single v376 ) )) ))) ))

(define (reduce redx)
        (let ((v1884 (filter-single redx ) ))
                 (let ((singles (concat v1884 ) ))
                          (reducehelp redx singles ) )))

(define (reducehelp redhx rsingles)
        (let ((v388 (null? redhx ) ))
                 (if v388 
                          () 
                         (let ((v1915 (car redhx ) ))
                                  (let ((v393 (minus v1915 rsingles ) ))
                                           (let ((v1908 (cdr redhx ) ))
                                                    (let ((v397 (reducehelp v1908 rsingles ) ))
                                                             (cons v393 v397 ) ))))) ))

(define (prune prux)
        (let ((v1933 (pruneby-rows prux ) ))
                 (let ((v402 (pruneby-cols v1933 ) ))
                          (pruneby-boxes v402 ) )))

(define (map-reduce mredx)
        (let ((v405 (null? mredx ) ))
                 (if v405 
                          () 
                         (let ((v1964 (car mredx ) ))
                                  (let ((v409 (reduce v1964 ) ))
                                           (let ((v1957 (cdr mredx ) ))
                                                    (let ((v412 (map-reduce v1957 ) ))
                                                             (cons v409 v412 ) ))))) ))

(define (pruneby-boxes prubx)
        (let ((v1982 (boxes prubx ) ))
                 (let ((v417 (map-reduce v1982 ) ))
                          (boxes v417 ) )))

(define (pruneby-cols prucx)
        (let ((v1996 (cols prucx ) ))
                 (let ((v421 (map-reduce v1996 ) ))
                          (cols v421 ) )))

(define (pruneby-rows prurx)
        (let ((v2010 (rows prurx ) ))
                 (let ((v425 (map-reduce v2010 ) ))
                          (rows v425 ) )))

(define (all-single asex)
        (let ((v428 (map-single asex ) ))
                 (alltrue v428 ) ))

(define (map-single msex)
        (let ((v431 (null? msex ) ))
                 (if v431 
                          () 
                         (let ((v2045 (car msex ) ))
                                  (let ((v435 (single v2045 ) ))
                                           (let ((v2038 (cdr msex ) ))
                                                    (let ((v438 (map-single v2038 ) ))
                                                             (cons v435 v438 ) ))))) ))

(define (all-all-single aasex)
        (let ((v442 (map-all-single aasex ) ))
                 (alltrue v442 ) ))

(define (map-all-single masex)
        (let ((v445 (null? masex ) ))
                 (if v445 
                          () 
                         (let ((v2084 (car masex ) ))
                                  (let ((v449 (all-single v2084 ) ))
                                           (let ((v2077 (cdr masex ) ))
                                                    (let ((v452 (map-all-single v2077 ) ))
                                                             (cons v449 v452 ) ))))) ))

(define (complete cmex)
        (all-all-single cmex ) )

(define (any-null anlx)
        (let ((v458 (null? anlx ) ))
                 (if v458 
                         #f 
                         (let ((v2107 (car anlx ) ))
                                  (let ((v2101 (null? v2107 ) ))
                                           (if v2101 
                                                   #t 
                                                   (let ((v465 (cdr anlx ) ))
                                                            (any-null v465 ) )) ))) ))

(define (any-any-null aanlx)
        (let ((v470 (null? aanlx ) ))
                 (if v470 
                         #f 
                         (let ((v2138 (car aanlx ) ))
                                  (let ((v2132 (any-null v2138 ) ))
                                           (if v2132 
                                                   #t 
                                                   (let ((v477 (cdr aanlx ) ))
                                                            (any-any-null v477 ) )) ))) ))

(define (void vx)
        (any-any-null vx ) )

(define (all-consistent actx)
        (let ((v484 (map-consistent actx ) ))
                 (alltrue v484 ) ))

(define (map-consistent mctx)
        (let ((v487 (null? mctx ) ))
                 (if v487 
                          () 
                         (let ((v2184 (car mctx ) ))
                                  (let ((v491 (consistent v2184 ) ))
                                           (let ((v2177 (cdr mctx ) ))
                                                    (let ((v494 (map-consistent v2177 ) ))
                                                             (cons v491 v494 ) ))))) ))

(define (consistent ctx)
        (let ((v2202 (filter-single ctx ) ))
                 (let ((v499 (concat v2202 ) ))
                          (nodups v499 ) )))

(define (safe sfex)
        (let ((v2293 (rows sfex ) ))
                 (let ((v503 (all-consistent v2293 ) ))
                          (let ((v2250 (cols sfex ) ))
                                   (let ((v2261 (all-consistent v2250 ) ))
                                            (let ((v2270 (boxes sfex ) ))
                                                     (let ((v2277 (all-consistent v2270 ) ))
                                                              (let ((v510 (and v2261 v2277 ) ))
                                                                       (and v503 v510 ) ))))))))

(define (blocked bkdx)
        (let ((v513 (null? bkdx ) ))
                 (let ((v2330 (void bkdx ) ))
                          (let ((v2339 (safe bkdx ) ))
                                   (let ((v2346 (not v2339 ) ))
                                            (let ((v519 (or v2330 v2346 ) ))
                                                     (or v513 v519 ) ))))))

(define (search m)
        (let ((v522 (blocked m ) ))
                 (if v522 
                          () 
                         (let ((v525 (complete m ) ))
                                  (if v525 
                                          (collapse m ) 
                                          (let ((m1 (expand m ) ))
                                                   (searchhelp m1 ) )) )) ))

(define (searchhelp m2)
        (let ((v536 (null? m2 ) ))
                 (if v536 
                          () 
                         (let ((v2400 (car m2 ) ))
                                  (let ((v2410 (prune v2400 ) ))
                                           (let ((s2 (search v2410 ) ))
                                                    (let ((v543 (single s2 ) ))
                                                             (if v543 
                                                                     s2 
                                                                     (let ((v546 (cdr m2 ) ))
                                                                              (searchhelp v546 ) )) ))))) ))

(define (not-single nsx)
        (let ((v552 (single nsx ) ))
                 (not v552 ) ))

(define (any-not-single ansx)
        (let ((v555 (null? ansx ) ))
                 (if v555 
                         #f 
                         (let ((v2449 (car ansx ) ))
                                  (let ((v2443 (not-single v2449 ) ))
                                           (if v2443 
                                                   #t 
                                                   (let ((v562 (cdr ansx ) ))
                                                            (any-not-single v562 ) )) ))) ))

(define (until-any-not-single uansx)
        (let ((v567 (null? uansx ) ))
                 (if v567 
                          () 
                         (let ((v2493 (car uansx ) ))
                                  (let ((v2480 (any-not-single v2493 ) ))
                                           (if v2480 
                                                    () 
                                                   (let ((v574 (car uansx ) ))
                                                            (let ((v2484 (cdr uansx ) ))
                                                                     (let ((v577 (until-any-not-single v2484 ) ))
                                                                              (cons v574 v577 ) )))) ))) ))

(define (after-any-not-single aansx)
        (let ((v582 (null? aansx ) ))
                 (if v582 
                          () 
                         (let ((v2528 (car aansx ) ))
                                  (let ((v2522 (any-not-single v2528 ) ))
                                           (if v2522 
                                                   aansx 
                                                   (let ((v589 (cdr aansx ) ))
                                                            (after-any-not-single v589 ) )) ))) ))

(define (until-not-single unsx)
        (let ((v594 (null? unsx ) ))
                 (if v594 
                          () 
                         (let ((v2572 (car unsx ) ))
                                  (let ((v2559 (not-single v2572 ) ))
                                           (if v2559 
                                                    () 
                                                   (let ((v601 (car unsx ) ))
                                                            (let ((v2563 (cdr unsx ) ))
                                                                     (let ((v604 (until-not-single v2563 ) ))
                                                                              (cons v601 v604 ) )))) ))) ))

(define (after-not-single afnsx)
        (let ((v609 (null? afnsx ) ))
                 (if v609 
                          () 
                         (let ((v2607 (car afnsx ) ))
                                  (let ((v2601 (not-single v2607 ) ))
                                           (if v2601 
                                                   afnsx 
                                                   (let ((v616 (cdr afnsx ) ))
                                                            (after-not-single v616 ) )) ))) ))

(define (expand em)
        (let ((vrows1 (until-any-not-single em ) ))
                 (let ((vrows2tmp (after-any-not-single em ) ))
                          (let ((vrow (car vrows2tmp ) ))
                                   (let ((vrows2 (cdr vrows2tmp ) ))
                                            (let ((vrow1 (until-not-single vrow ) ))
                                                     (let ((vrow2tmp (after-not-single vrow ) ))
                                                              (let ((vcs (car vrow2tmp ) ))
                                                                       (let ((vrow2 (cdr vrow2tmp ) ))
                                                                                (expandhelp vrows1 vrow1 vrow2 vrows2 vcs ) )))))))))

(define (expandhelp vrs1 vr1 vr2 vrs2 ehx)
        (let ((v651 (null? ehx ) ))
                 (if v651 
                          () 
                         (let ((v2682 (car ehx ) ))
                                  (let ((v2695 (list v2682 ) ))
                                           (let ((v2706 (cons v2695 vr2 ) ))
                                                    (let ((vnr (append vr1 v2706 ) ))
                                                             (let ((v2644 (cons vnr vrs2 ) ))
                                                                      (let ((vnb (append vrs1 v2644 ) ))
                                                                               (let ((v2649 (cdr ehx ) ))
                                                                                        (let ((v672 (expandhelp vrs1 vr1 vr2 vrs2 v2649 ) ))
                                                                                                 (cons vnb v672 ) ))))))))) ))

(define (solve slvx)
        (let ((v2739 (choices slvx ) ))
                 (let ((v679 (prune v2739 ) ))
                          (search v679 ) )))

(define (split spx)
        (let ((boxsize 3 ))
                 (chop boxsize spx ) ))

(define (choice chv)
        (let ((v2853 (cons 9  () ) ))
                 (let ((v2839 (cons 8 v2853 ) ))
                          (let ((v2826 (cons 7 v2839 ) ))
                                   (let ((v2814 (cons 6 v2826 ) ))
                                            (let ((v2803 (cons 5 v2814 ) ))
                                                     (let ((v2793 (cons 4 v2803 ) ))
                                                              (let ((v2784 (cons 3 v2793 ) ))
                                                                       (let ((v2776 (cons 2 v2784 ) ))
                                                                                (let ((vals (cons 1 v2776 ) ))
                                                                                         (let ((v706 (empty chv ) ))
                                                                                                  (if v706 
                                                                                                          vals 
                                                                                                          (list chv ) ) )))))))))))

(let ((v7852 (cons 3  () ) ))
         (let ((v7759 (cons 0 v7852 ) ))
                  (let ((v7667 (cons 2 v7759 ) ))
                           (let ((v7576 (cons 8 v7667 ) ))
                                    (let ((v7486 (cons 0 v7576 ) ))
                                             (let ((v7397 (cons 1 v7486 ) ))
                                                      (let ((v7309 (cons 0 v7397 ) ))
                                                               (let ((v7222 (cons 0 v7309 ) ))
                                                                        (let ((v3136 (cons 6 v7222 ) ))
                                                                                 (let ((v7136 (cons 0  () ) ))
                                                                                          (let ((v7052 (cons 9 v7136 ) ))
                                                                                                   (let ((v6969 (cons 0 v7052 ) ))
                                                                                                            (let ((v6887 (cons 0 v6969 ) ))
                                                                                                                     (let ((v6806 (cons 4 v6887 ) ))
                                                                                                                              (let ((v6726 (cons 0 v6806 ) ))
                                                                                                                                       (let ((v6647 (cons 0 v6726 ) ))
                                                                                                                                                (let ((v6569 (cons 2 v6647 ) ))
                                                                                                                                                         (let ((v3307 (cons 0 v6569 ) ))
                                                                                                                                                                  (let ((v6492 (cons 0  () ) ))
                                                                                                                                                                           (let ((v6417 (cons 0 v6492 ) ))
                                                                                                                                                                                    (let ((v6343 (cons 4 v6417 ) ))
                                                                                                                                                                                             (let ((v6270 (cons 5 v6343 ) ))
                                                                                                                                                                                                      (let ((v6198 (cons 0 v6270 ) ))
                                                                                                                                                                                                               (let ((v6127 (cons 0 v6198 ) ))
                                                                                                                                                                                                                        (let ((v6057 (cons 3 v6127 ) ))
                                                                                                                                                                                                                                 (let ((v5988 (cons 0 v6057 ) ))
                                                                                                                                                                                                                                          (let ((v3460 (cons 8 v5988 ) ))
                                                                                                                                                                                                                                                   (let ((v5920 (cons 9  () ) ))
                                                                                                                                                                                                                                                            (let ((v5854 (cons 0 v5920 ) ))
                                                                                                                                                                                                                                                                     (let ((v5789 (cons 0 v5854 ) ))
                                                                                                                                                                                                                                                                              (let ((v5725 (cons 7 v5789 ) ))
                                                                                                                                                                                                                                                                                       (let ((v5662 (cons 0 v5725 ) ))
                                                                                                                                                                                                                                                                                                (let ((v5600 (cons 6 v5662 ) ))
                                                                                                                                                                                                                                                                                                         (let ((v5539 (cons 4 v5600 ) ))
                                                                                                                                                                                                                                                                                                                  (let ((v5479 (cons 0 v5539 ) ))
                                                                                                                                                                                                                                                                                                                           (let ((v3595 (cons 5 v5479 ) ))
                                                                                                                                                                                                                                                                                                                                    (let ((v5420 (cons 0  () ) ))
                                                                                                                                                                                                                                                                                                                                             (let ((v5363 (cons 5 v5420 ) ))
                                                                                                                                                                                                                                                                                                                                                      (let ((v5307 (cons 0 v5363 ) ))
                                                                                                                                                                                                                                                                                                                                                               (let ((v5252 (cons 0 v5307 ) ))
                                                                                                                                                                                                                                                                                                                                                                        (let ((v5198 (cons 0 v5252 ) ))
                                                                                                                                                                                                                                                                                                                                                                                 (let ((v5145 (cons 0 v5198 ) ))
                                                                                                                                                                                                                                                                                                                                                                                          (let ((v5093 (cons 0 v5145 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                   (let ((v5042 (cons 3 v5093 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                            (let ((v3712 (cons 0 v5042 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((v4992 (cons 2  () ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((v4944 (cons 0 v4992 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                       (let ((v4897 (cons 1 v4944 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                (let ((v4851 (cons 3 v4897 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((v4806 (cons 0 v4851 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                  (let ((v4762 (cons 8 v4806 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                           (let ((v4719 (cons 0 v4762 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((v4677 (cons 0 v4719 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             (let ((v3811 (cons 7 v4677 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      (let ((v4636 (cons 6  () ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((v4597 (cons 0 v4636 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (let ((v4559 (cons 9 v4597 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 (let ((v4522 (cons 0 v4559 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          (let ((v4486 (cons 0 v4522 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   (let ((v4451 (cons 7 v4486 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            (let ((v4417 (cons 1 v4451 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((v4384 (cons 0 v4417 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((v3892 (cons 0 v4384 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       (let ((v4352 (cons 0  () ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                (let ((v4322 (cons 2 v4352 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((v4293 (cons 0 v4322 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  (let ((v4265 (cons 0 v4293 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           (let ((v4238 (cons 3 v4265 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((v4212 (cons 0 v4238 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             (let ((v4187 (cons 0 v4212 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      (let ((v4163 (cons 8 v4187 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((v3955 (cons 0 v4163 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (let ((v4140 (cons 5  () ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 (let ((v4119 (cons 0 v4140 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          (let ((v4099 (cons 0 v4119 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   (let ((v4080 (cons 4 v4099 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            (let ((v4062 (cons 0 v4080 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     (let ((v4045 (cons 9 v4062 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              (let ((v4029 (cons 2 v4045 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       (let ((v4014 (cons 0 v4029 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                (let ((v4000 (cons 3 v4014 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         (let ((v3987 (cons v4000  () ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  (let ((v3943 (cons v3955 v3987 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           (let ((v3881 (cons v3892 v3943 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    (let ((v3801 (cons v3811 v3881 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             (let ((v3703 (cons v3712 v3801 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      (let ((v3587 (cons v3595 v3703 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (let ((v3453 (cons v3460 v3587 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (let ((v3301 (cons v3307 v3453 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 (let ((b1 (cons v3136 v3301 ) ))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          (solve b1 ) ))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

