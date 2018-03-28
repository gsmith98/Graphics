/////////////////////
// Animation stuff //
/////////////////////
template <class Vector>
void ParameterSamples<Vector>::setCurrentValue(double t,int type){
	switch (type){
		case LINEAR:
			currentValue=samples[0] + (samples[1] - samples[0])*t;
			break;
		case CATMULL_ROM:
			currentValue = samples[0] * (-.5*t*t*t + t*t - .5*t)
				+ samples[1] * (1.5*t*t*t - 2.5*t*t + 1)
				+ samples[2] * (-1.5*t*t*t + 2 * t*t + .5*t)
				+ samples[3] * (.5*t*t*t - .5*t*t);
			break;
		case UNIFORM_CUBIC_B_SPLINE: //This one will lose the outer keyframe points (expected) so dude will walk less or whichever is being tested
			currentValue = samples[0] * (1.0 / 6.0)*(1 - t)*(1 - t)*(1 - t)
				+ samples[1] * (1.0 / 6.0)*(3 * t*t*t - 6 * t*t + 4)
				+ samples[2] * (1.0 / 6.0)*(-3 * t*t*t + 3 * t*t + 3 * t + 1)
				+ samples[3] * (1.0 / 6.0) * t*t*t;
			break;
	};
}
