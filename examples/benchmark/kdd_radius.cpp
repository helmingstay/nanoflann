/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2011-2016 Jose Luis Blanco (joseluisblancoc@gmail.com).
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/
#include <cstdlib>
#include "kdd_radius.h"

pcloud_t generateRandomPointCloud(const size_t N, rng_t rng, const num_t from, const num_t to)
{
   std::uniform_real_distribution<double> distrib(from,to);
    if (!quiet) {
        cout << "Generating "<< N << " point cloud." << endl;
    }
    pcloud_t ret(new PointCloud);
	ret->pts.resize(N);
	for (size_t ii=0;ii<N;ii++) {
        for (size_t jj=0;jj<the_dim;jj++) {
            ret->pts[ii][jj] = distrib(rng);
        }
	}
    return ret;
}

ptree_t build_tree(pcloud_t & pcloud) {
    // initialize tree from cloud, 
    // store as unique pointer
	ptree_t ret = ptree_t(
        new kd_tree_t(
            the_dim-1, *pcloud,
            // max leaf 
            KDTreeSingleIndexAdaptorParams(10) 
        )
    );
	ret->buildIndex();
    #if 0
        // Test resize of dataset and rebuild of index:
        ret->pts.resize(ret->pts.size()*0.5);
        ret->buildIndex();
    #endif
    return ret;
}

	// ----------------------------------------------------------------
	// radiusSearch():  Perform a search for the N closest points
	// ----------------------------------------------------------------
void search_tree(cloudtree dat,
    const Point query_pt,
    const num_t search_radius,
    size_t nsearch
) {
    // result vector
    results_t matches;
    nanoflann::SearchParams params;
    //params.sorted = false;
    size_t nMatch;
    for (size_t ii=0; ii<nsearch; ii++) {
        // run the search
        nMatch = dat.ptree->radiusSearch(
            query_pt.data(), search_radius, matches, params);

        if (!quiet) {
            cout 
                << "radius= " << search_radius 
                << ", matches= " << nMatch
                << endl
            ;
        }
    }
}

void print_usage(const char * name) noexcept {
    cout << "Usage: " << name << " npoint dist nsearch" << endl;
}

int main(int argc, char *argv[])
{
    using tpoint = float;
    // commandline args
    long npoint{0}, nsearch{0};
    tpoint dist;
    // check & convert
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }
    try {
        npoint = std::stol(argv[1]);
        dist = std::stod(argv[2]);
        nsearch = std::stol(argv[3]);
    } catch (...) {
        print_usage(argv[0]);
        cout << "Error parsing commandline args" << endl;
        return 1;
    }
        
	// RNG init
	// From http://www.cplusplus.com/reference/random/mersenne_twister_engine/mersenne_twister_engine/
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    rng_t rng(seed1);
    // complete data structure: points + cloud
    cloudtree dat;

	// Generate points:
	dat.pcloud = generateRandomPointCloud(npoint, rng, cloud_min, cloud_max);

    // 
    const Point query_pt{ 0.0, 0.0, 0.0};
    // initialize pointcloud, tree. 
    // return as struct
    dat.ptree = build_tree(dat.pcloud);
	search_tree(std::move(dat), query_pt, dist, nsearch);
	return 0;
}
