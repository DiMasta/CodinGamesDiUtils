//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

#ifdef USE_UNIFORM_RANDOM
random_device rd;
mt19937 mt(rd());
uniform_real_distribution<float> dist(0.0, 1.0);
#endif

namespace Math {
#ifdef USE_UNIFORM_RANDOM
	float randomFloatBetween0and1() {
		return dist(mt);
	}
#else
	float randomFloatBetween0and1() {
		return static_cast<float>(rand()) / FLOAT_MAX_RAND;
	}
#endif
};