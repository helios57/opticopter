package ch.sharpsoft.opticopterandroid;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.FpsMeter;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Canvas;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.hardware.usb.UsbAccessory;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.ParcelFileDescriptor;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Toast;

public class MainActivity extends Activity implements SensorEventListener {

	private static final String ACTION_USB_PERMISSION = "com.google.android.DemoKit.action.USB_PERMISSION";

	private static final String TAG = "OpticopterAndroidJava::Activity";
	private final byte preamble0 = (byte) 0xE5;
	private final byte preamble1 = (byte) 0xE7;

	private Handler handler;
	private UsbManager usbManager;
	private PendingIntent mPermissionIntent;

	private UsbAccessory accessory;
	private ParcelFileDescriptor mFileDescriptor;
	private FileInputStream mInputStream;
	private FileOutputStream mOutputStream;

	private static final Scalar DETECTED_OBJ_RECT_COLOR = new Scalar(0, 255, 0, 255);
	public static final int JAVA_DETECTOR = 0;
	public static final int NATIVE_DETECTOR = 1;

	private MenuItem mItemFace50;

	private File mCascadeFile;
	private OpticopterAndroid mNativeDetector;
	private VideoCapture videoCapture;
	private Mat mGray;
	private SurfaceView surfaceView;
	private final Bitmap mCacheBitmap = Bitmap.createBitmap(1280, 720, Bitmap.Config.ARGB_8888);
	private final Object bitmapSendLock = new Object();
	private FpsMeter mFpsMeter;
	private SensorManager mSensorManager;
	private Sensor mRotation;
	private final Server server = new Server();
	private final float[] rotation = new float[3];
	private final float[] currentRot = new float[4];
	private final float[] leveled = new float[4];

	// private final double pprh = 0.0014097904947;// rad/px
	private final double ppr = 0.0008870389392;// rad/px

	private volatile boolean grabbing = false;

	private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (ACTION_USB_PERMISSION.equals(action)) {
				synchronized (this) {
					UsbAccessory laccessory = (UsbAccessory) intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
					if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
						if (laccessory != null) {
							accessory = laccessory;
							openAccessory();
						}
					} else {
						Log.d(TAG, "permission denied for accessory " + laccessory);
					}
				}
			}
		}
	};

	private final Runnable updateRunnable = new Runnable() {

		@Override
		public void run() {
			while (grabbing && videoCapture.grab()) {
				videoCapture.retrieve(mGray, Highgui.CV_CAP_ANDROID_GREY_FRAME);
				final float[] diff = quatDiff(currentRot, leveled);
				final double hRad = Math.asin(2 * (diff[0] * diff[2] - diff[1] * diff[3]));
				final double wRad = Math.atan2(2 * (diff[0] * diff[1] + diff[2] * diff[3]), 1 - 2 * (diff[1] * diff[1] + diff[2] * diff[2]));
				final double yaw = Math.atan2(2 * (diff[0] * diff[3] + diff[1] * diff[2]), 1 - 2 * (diff[2] * diff[2] + diff[3] * diff[3]));

				final Mat roi = mGray;
				final MatOfRect detectedObjs = new MatOfRect();
				mNativeDetector.detect(roi, detectedObjs);
				final Rect[] detectedObjsArray = detectedObjs.toArray();
				Rect biggest = null;
				for (int i = 0; i < detectedObjsArray.length; i++) {
					final Rect rect = detectedObjsArray[i];
					if (biggest == null || biggest.width * biggest.height < rect.width * rect.height) {
						biggest = rect;
					}
					Core.rectangle(roi, rect.tl(), rect.br(), DETECTED_OBJ_RECT_COLOR, 3);
				}
				if (biggest != null) {
					int x = biggest.x + biggest.width / 2;
					int y = biggest.y + biggest.height / 2;

					// double hdiff = ((720 / 2) - y) * pprh + hRad;
					double hdiff = ((720 / 2) - y) * ppr + hRad;
					double wdiff = ((1280 / 2) - x) * ppr - wRad;

					if (mOutputStream != null) {
						final ByteBuffer out = ByteBuffer.allocate(16);
						out.order(ByteOrder.LITTLE_ENDIAN);
						out.put(0, preamble0);
						out.put(1, preamble1);
						out.put(2, (byte) 0x03);
						out.putFloat(3, (float) hdiff); // roll
						out.putFloat(7, (float) -wdiff);// pitch
						out.putFloat(11, (float) yaw);// yaw to level
						byte outSum = 0;
						for (int i = 2; i < 15; i++) {
							outSum += out.get(i);
						}
						out.put(15, outSum);
						synchronized (mOutputStream) {
							try {
								mOutputStream.write(out.array());
							} catch (IOException e) {
							}
						}
					}

					Core.putText(roi, "x=" + x + " y=" + y, new Point(0, 300), Core.FONT_HERSHEY_SIMPLEX, 1.0, new Scalar(255, 255, 255));
					Core.putText(roi, "hdiff=" + hdiff + " wdiff=" + wdiff, new Point(0, 450), Core.FONT_HERSHEY_SIMPLEX, 1.0, new Scalar(255, 255, 255));
				}

				// TODO scale
				synchronized (mCacheBitmap) {
					Utils.matToBitmap(roi, mCacheBitmap);
					mCacheBitmap.notifyAll();
				}
				Canvas canvas = surfaceView.getHolder().lockCanvas();
				if (canvas != null) {
					canvas.drawColor(0, android.graphics.PorterDuff.Mode.CLEAR);
					canvas.drawBitmap(mCacheBitmap, new android.graphics.Rect(0, 0, mCacheBitmap.getWidth(), mCacheBitmap.getHeight()),
							new android.graphics.Rect((canvas.getWidth() - mCacheBitmap.getWidth()) / 2, (canvas.getHeight() - mCacheBitmap.getHeight()) / 2, (canvas.getWidth() - mCacheBitmap.getWidth()) / 2 + mCacheBitmap.getWidth(),
									(canvas.getHeight() - mCacheBitmap.getHeight()) / 2 + mCacheBitmap.getHeight()), null);

					mFpsMeter.measure();
					mFpsMeter.draw(canvas, 20, 30);
					surfaceView.getHolder().unlockCanvasAndPost(canvas);
				}

				SensorManager.getQuaternionFromVector(currentRot, rotation);
			}
		};
	};

	private final Runnable sendRunnable = new Runnable() {

		@Override
		public void run() {
			while (true) {
				try {
					Bitmap copy;
					synchronized (mCacheBitmap) {
						mCacheBitmap.wait();
						copy = mCacheBitmap.copy(mCacheBitmap.getConfig(), false);
					}
					synchronized (bitmapSendLock) {
						sendBitmap(copy);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}

		}
	};
	private final BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
			case LoaderCallbackInterface.SUCCESS: {
				Log.i(TAG, "OpenCV loaded successfully");
				// Load native library after(!) OpenCV initialization
				System.loadLibrary("opticopter_android");
				mGray = new Mat();
				mFpsMeter = new FpsMeter();

				try {
					InputStream is = getResources().openRawResource(R.raw.lbpcascade_opticopter);
					File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
					mCascadeFile = new File(cascadeDir, "lbpcascade_opticopter.xml");
					FileOutputStream os = new FileOutputStream(mCascadeFile);

					byte[] buffer = new byte[4096];
					int bytesRead;
					while ((bytesRead = is.read(buffer)) != -1) {
						os.write(buffer, 0, bytesRead);
					}
					is.close();
					os.close();

					mNativeDetector = new OpticopterAndroid(mCascadeFile.getAbsolutePath(), 0);

					cascadeDir.delete();

				} catch (IOException e) {
					e.printStackTrace();
					Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
				}

				videoCapture = new VideoCapture(Highgui.CV_CAP_ANDROID);
				videoCapture.set(Highgui.CV_CAP_PROP_FRAME_WIDTH, 1280);
				videoCapture.set(Highgui.CV_CAP_PROP_FRAME_HEIGHT, 720);
				grabbing = true;
				server.init();
				new Thread(updateRunnable).start();
				new Thread(sendRunnable).start();
			}
				break;
			default: {
				super.onManagerConnected(status);
			}
				break;
			}
		}
	};

	public MainActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "called onCreate");
		super.onCreate(savedInstanceState);
		handler = new Handler();
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
		mRotation = mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
		Button level = new Button(this);
		level.setText("Level");
		level.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				SensorManager.getQuaternionFromVector(leveled, rotation);
			}
		});

		final LinearLayout ll = new LinearLayout(this);
		ll.setOrientation(LinearLayout.VERTICAL);
		setContentView(ll);
		ll.addView(level);
		surfaceView = new SurfaceView(this);
		ll.addView(surfaceView);
		usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
		accessory = (UsbAccessory) getIntent().getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
		if (accessory == null && usbManager != null) {
			final UsbAccessory[] list = usbManager.getAccessoryList();
			if (list != null) {
				for (UsbAccessory a : list) {
					accessory = a;
				}
			}
		}
		mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
		IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
		registerReceiver(mUsbReceiver, filter);
		if (accessory != null) {
			usbManager.requestPermission(accessory, mPermissionIntent);
		}
	}

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		accessory = (UsbAccessory) intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
		if (accessory != null) {
			usbManager.requestPermission(accessory, mPermissionIntent);
		}
	}

	private void openAccessory() {
		Log.d(TAG, "openAccessory: " + accessory);
		Toast.makeText(this, "openAccessory " + accessory.getDescription(), Toast.LENGTH_LONG).show();
		mFileDescriptor = usbManager.openAccessory(accessory);
		if (mFileDescriptor != null) {
			FileDescriptor fd = mFileDescriptor.getFileDescriptor();
			mInputStream = new FileInputStream(fd);
			mOutputStream = new FileOutputStream(fd);
			new Thread(new Runnable() {
				private final byte[] buffer = new byte[16];

				@Override
				public void run() {
					while (mInputStream != null) {
						try {
							final int read = mInputStream.read(buffer);
							if (read == 16 && preamble0 == buffer[0] && preamble1 == buffer[1]) {
								byte sum = 0;
								for (int i = 2; i < 15; i++) {
									sum += buffer[i];
								}
								if (sum == buffer[15]) {
									final ByteBuffer bb = ByteBuffer.wrap(buffer);
									bb.order(ByteOrder.LITTLE_ENDIAN);
									final byte flags = bb.get(2);
									final float v0 = bb.getFloat(3);
									final float v1 = bb.getFloat(7);
									final float v2 = bb.getFloat(11);
									// TODO use

								} else {
									handler.post(new Runnable() {
										@Override
										public void run() {
											Toast.makeText(MainActivity.this, "Wrong checksum", Toast.LENGTH_SHORT).show();
										}
									});
								}
							} else {
								handler.post(new Runnable() {
									@Override
									public void run() {
										Toast.makeText(MainActivity.this, "Wrong preamble", Toast.LENGTH_SHORT).show();
									}
								});
							}
						} catch (final Exception e) {
							handler.post(new Runnable() {
								@Override
								public void run() {
									Toast.makeText(MainActivity.this, e.getMessage(), Toast.LENGTH_LONG).show();
								}
							});
							try {
								Thread.sleep(10000);
							} catch (InterruptedException e1) {
							}
						}
					}
				}
			}).start();
		}
	}

	@Override
	public void onPause() {
		super.onPause();
		grabbing = false;
		if (videoCapture != null) {
			videoCapture.release();
		}
		mSensorManager.unregisterListener(this);
	}

	@Override
	public void onResume() {
		super.onResume();
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_7, this, mLoaderCallback);
		mSensorManager.registerListener(this, mRotation, SensorManager.SENSOR_DELAY_GAME);
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		grabbing = false;
		if (videoCapture != null && videoCapture.isOpened()) {
			videoCapture.release();
		}
		if (server != null) {
			server.close();
		}
		unregisterReceiver(mUsbReceiver);
		if (mInputStream != null) {
			try {
				mInputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			mInputStream = null;
		}
		if (mOutputStream != null) {
			try {
				mOutputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			mOutputStream = null;
		}
		if (mFileDescriptor != null) {
			try {
				mFileDescriptor.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			mFileDescriptor = null;
		}

	}

	float[] quatDiff(float[] rot, float[] level) {
		float[] result = new float[4];
		result[0] = rot[0] * level[0] - -rot[1] * level[1] - -rot[2] * level[2] - -rot[3] * level[3];
		result[1] = rot[0] * level[1] + -rot[1] * level[0] + -rot[2] * level[3] - -rot[3] * level[2];
		result[2] = rot[0] * level[2] - -rot[1] * level[3] + -rot[2] * level[0] + -rot[3] * level[1];
		result[3] = rot[0] * level[3] + -rot[1] * level[2] - -rot[2] * level[1] + -rot[3] * level[0];

		double norm = Math.sqrt(result[0] * result[0] + result[1] * result[1] + result[2] * result[2] + result[3] * result[3]);
		for (int i = 0; i < 4; i++) {
			result[0] /= norm;
		}
		return result;

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		Log.i(TAG, "called onCreateOptionsMenu");
		mItemFace50 = menu.add("Face size 50%");
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Log.i(TAG, "called onOptionsItemSelected; selected item: " + item);
		if (item == mItemFace50) {
			// setMinFaceSize(0.5f);
		}
		return true;
	}

	@Override
	public void onSensorChanged(final SensorEvent event) {
		if (event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR) {
			for (int i = 0; i < 3; i++) {
				rotation[i] = event.values[i];
			}
		}
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
	}

	private void sendBitmap(final Bitmap copy) {
		final OutputStream os = server.getOS();
		if (os != null) {
			try {
				os.write(new byte[] { 111, 22, 33, 44, 55 });
				copy.compress(CompressFormat.JPEG, 90, os);
			} catch (Exception e) {
				server.reset();
				e.printStackTrace();
			}
		}
	}
}
