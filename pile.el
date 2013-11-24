(lazypile/init)

;; c++
(lazypile/make 
 "cpp" 
 (lambda () 
   (lazypile/find-files 
    "find -E '.' -regex '.*\.(h|cpp)' -print")))

