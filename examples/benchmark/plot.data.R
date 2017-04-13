source('read.data.R')
library('ggplot2')

## convenience: 
## prep for ggplot discrete scale
## preserve order 
.fac.fun <- function(label, vec) {
    ret <- paste0(label, vec)
    ## preserve order
    ret <- factor(ret, levels=unique(ret))
}

## cleanup data
dat.plot <- subset(dat, user>0 & radius >0)
dat.plot <- within(dat.plot,{
    radius.f <- .fac.fun('Radius: ', radius)
    nsearch.f <- .fac.fun('Nsearch: ', nsearch)
})

## the plot
.pp <- (
    ggplot(dat.plot, aes(x=npoint, y=user, color=radius.f))
    + facet_grid(group ~ nsearch.f)
    + geom_line()
    + geom_point()
    + theme_bw(base_size=24)
    + scale_x_log10('Pointcloud size')
    + scale_y_log10('User Time (seconds)')
    + scale_color_brewer('Search Radius\n(over unit box)',type='qual', palette='Set1')
    + theme(
        legend.position=c(0.01,0.99),
        legend.justification=c(0,1)
    )
)
plot(.pp)
