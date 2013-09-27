package ch.sharpsoft.arducopter.client.model;

public class PID {
	double pgain, igain, dgain, imax;
	double pTerm, iTerm, dTerm;
	double iState;
	double last;
	double diff;

	public PID(final double p, final double i, final double d) {
		pgain = p;
		igain = i;
		dgain = d;
		imax = Math.PI / 8;
	}

	public double updatePID(final double target, final double cur, final long deltaTime) {
		diff = target - cur;

		// the pTerm is the view from now, the pgain judges
		// how much we care about error at this instant.
		pTerm = pgain * diff;

		// integrated iState
		iState += diff * (deltaTime / 1000.0);

		// limit iTerm
		if (iState > imax)
			iState = imax;
		else if (iState < -imax)
			iState = -imax;

		iTerm = igain * iState;

		// speed / Differential
		dTerm = (dgain * (cur - last)) / deltaTime;
		last = cur;
		return pTerm + iTerm - dTerm;
	}
}
