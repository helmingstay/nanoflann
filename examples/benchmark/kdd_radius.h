#include <array>
#include <memory>
#include <random>
#include <iostream>
#include <chrono>
#include <nanoflann.hpp>

using std::array;
using std::cout;
using std::endl;
using namespace nanoflann;

// change to see timings
constexpr size_t the_dim = 3;
using num_t = float;
constexpr num_t cloud_min = -10;
constexpr num_t cloud_max = 10;
// don't print 
constexpr bool quiet = false;


// basic types
using Point = array<num_t, the_dim>;
using results_t = std::vector<std::pair<size_t,num_t> >;
using rng_t = std::mt19937;

// This is an example of a custom data set class
struct PointCloud {
	std::vector<Point> pts;

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
	inline num_t kdtree_distance(const num_t *p1, const size_t idx_p2,size_t /*size*/) const
	{
        num_t dist(0);
        for (size_t ii=0; ii<the_dim; ii++) {
            const num_t d0=p1[ii]-pts[idx_p2][ii];
            dist += (d0*d0);
        }
		return dist;
	}


	inline num_t kdtree_get_pt(const size_t idx, size_t _dim) const {
        if (_dim >= the_dim) {
            throw std::runtime_error("Bad dim");
        }
        return pts[idx][_dim];
    }

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /*bb*/) const { return false; }
};

// kd-tree index:
using kd_tree_t = KDTreeSingleIndexAdaptor<
    L2_Simple_Adaptor<num_t, PointCloud>,
    PointCloud, the_dim
>;

using ptree_t = std::unique_ptr<kd_tree_t>;
using pcloud_t = std::unique_ptr<PointCloud>;

// encapsulated return value 
struct cloudtree {
    pcloud_t pcloud;
    ptree_t ptree;
};
