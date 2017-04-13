## data.table is fast
library('data.table')

## user-specified
result.dir <- './results'

## file patterns from benchmark.sh
patterns <- list(
    cond='conditions', time='timings'
)

## for each pattern: 
## -- get list of files
## -- read files, rbind into single DT
ldat <- lapply(patterns, function(name) {
    fn <- dir(path=result.dir, pattern=name, full.names=T)
    group <- basename(fn)
    group <- gsub(name, '', group)
    group <- gsub('[.]|csv', '', group)
    ## 
    ret <- rbindlist(
        lapply(fn, function(x) fread(x))
    )
    ret[, group := factor(group)]
    ret[, id := (1:nrow(ret))]
})
    
## merge into final 
dat <- with(ldat, 
    merge(cond, time, by=c('id','group'))
)
