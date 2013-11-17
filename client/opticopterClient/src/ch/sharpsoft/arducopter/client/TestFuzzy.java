package ch.sharpsoft.arducopter.client;

import net.sourceforge.jFuzzyLogic.FIS;

public class TestFuzzy {
	public static void main(final String[] args) throws Exception {
		// Load from 'FCL' file
		FIS fis = FIS.load("C:/Users/Helios/git/opticopter/client/opticopterClient/src/ch/sharpsoft/arducopter/client/fuzzy.fcl", true);

		// Error while loading?
		if (fis == null) {
			System.err.println("Can't load file:");
			return;
		}

		// Set inputs
		fis.setVariable("diff", 0.2);
		fis.setVariable("rate", +1);
		fis.setVariable("integradedDiff", 1);

		// Evaluate
		fis.evaluate();

		// Show output variable's chart
		double thrust = fis.getVariable("thrust").defuzzify();

		// Print ruleSet
		System.out.println(fis);
		System.out.println(thrust);
	}
}