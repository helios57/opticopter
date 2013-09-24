package ch.sharpsoft.arducopter.client.view;

public class Matrix {
	public static void plus(final double[][] a, final double[][] b, final double[][] out) {
		for (int i = 0; i < a.length; i++) {
			for (int j = 0; j < a[i].length; j++) {
				out[i][j] = a[i][j] + b[i][j];
			}
		}
	}

	public static void minus(final double[][] a, final double[][] b, final double[][] out) {
		for (int i = 0; i < a.length; i++) {
			for (int j = 0; j < a[i].length; j++) {
				out[i][j] = a[i][j] - b[i][j];
			}
		}
	}

	public static void multiply(final double[][] a, final double[][] b, final double[][] out) {
		double[][] result = new double[a.length][a[0].length];
		int k;
		// Handles rows of left Matrix
		for (int i = 0; i < a.length; i++) {
			// Handles columns of right Matrix
			for (int j = 0; j < b.length; j++) {
				// Handles the multiplication
				for (k = 0, out[i][j] = 0.0; k < a[i].length; k++) {
					result[i][j] += a[i][k] * b[k][j];
				}
			}
		}
		for (int i = 0; i < a.length; i++) {
			// Handles columns of right Matrix
			for (int j = 0; j < a[0].length; j++) {
				// Handles the multiplication
				out[i][j] = result[i][j];
			}
		}
	}

	public static void invert3x3(final double[][] a, final double[][] out) {
		double[][] result = new double[a.length][a[0].length];
		double det = a[0][0] * a[1][1] * a[2][2] + a[0][1] * a[1][2] * a[2][0] + a[0][2] * a[1][0] * a[2][1] - a[0][2] * a[1][1] * a[2][0] - a[1][2] * a[2][1] * a[0][0] - a[2][2] * a[0][1] * a[1][0];

		result[0][0] = (a[1][1] * a[2][2] - a[1][2] * a[2][1]) / det;
		result[0][1] = (a[0][2] * a[2][1] - a[0][1] * a[2][2]) / det;
		result[0][2] = (a[0][1] * a[2][1] - a[1][1] * a[2][0]) / det;

		result[1][0] = (a[1][2] * a[2][0] - a[1][0] * a[2][2]) / det;
		result[1][1] = (a[0][0] * a[2][2] - a[0][2] * a[2][0]) / det;
		result[1][2] = (a[0][2] * a[1][0] - a[0][0] * a[1][2]) / det;

		result[2][0] = (a[1][0] * a[2][1] - a[1][1] * a[2][0]) / det;
		result[2][1] = (a[0][1] * a[2][0] - a[0][0] * a[2][1]) / det;
		result[2][2] = (a[0][0] * a[1][1] - a[0][1] * a[1][0]) / det;

		for (int i = 0; i < a.length; i++) {
			// Handles columns of right Matrix
			for (int j = 0; j < a[0].length; j++) {
				// Handles the multiplication
				out[i][j] = result[i][j];
			}
		}
	}
}
