// Implements a Persistence-of-Vision display, deriving the data
// from uploaded .magicBitmap files
// files may be uploaded/modified/deleted by the user through the
// web interface, so we refresh our onboard POV files with every
// user event (except of course, Shaking..)
 

#define FRAME_MULTIPLY 2

#define MS_SHAKEFILE_DEFAULT "nix"

// We use a bouncing ball during non-shake periods as a 'screensaver' to
// nevertheless indicate that the MagicShifter is operational in this
// mode
BouncingBallMode msModeBouncingBall(600);

class MagicShakeMode : public MagicShifterBaseMode
{
private:
  // The direction through the filelist which the user is scrolling (-ve/+ve)
  int dirCursor = 0;

  // the last frame of the Shake
  MagicShifterImageText msMagicShakeText;

  MagicPOVMode lPOVMode;
  MagicShifterImage lLocalImage;

  // the number of files discovered onboard during the scan for POV images..
  int numFiles = 0;
  bool correctBrightness = false;
  bool shouldDisplayText=false;

public:
  const char *modeName="MagicShake";

  MagicShakeMode()
  {
  }

  // Get a file from the list of onboard files, filtering only .magicBitmap files
  // fileIndex: the idx of the file in the list
  // maxFiles: returns the length of the list
  // return: filename when found, empty string when not found
  String getFileNameAtIndex(int fileIndex, int &maxFiles)
  {
    Dir POVDir;
    msSystem.slog("getFileNameAtIndex:"); msSystem.slogln(String(fileIndex));
    POVDir = SPIFFS.openDir("");

    int cnt = 0;

    String foundFilename = "";
    String currentFilename = "";

    while(1)
    {
      if (!POVDir.next()) 
        break; // end of list
      
      currentFilename = POVDir.fileName();

      if (!currentFilename.endsWith(".magicBitmap")) 
        continue;

      if (cnt == fileIndex)
        foundFilename = currentFilename;

      cnt++;
    }

    maxFiles = cnt;

    return foundFilename;
  }

  // load a magic Shake file for display
  void loadShakeFile(const char *filename)
  {
    msSystem.slog("loadShakeFile:"); msSystem.slogln(filename);

    lLocalImage.close();
    lLocalImage.LoadFile(filename);
    lPOVMode.setImage(&lLocalImage);
  }

  // Start the MagicShake mode:
  //  shake the last-uploaded .magicBitmap (if set)
  //  prime the file list, which may update dynamically during our session
  void start()
  {

    // lPOVMode.start();

    // 0 = picshow, 1 = textshow
    shouldDisplayText = false;

    if (String(msGlobals.ggUploadFileName).endsWith(".magicBitmap")) {
      loadShakeFile(msGlobals.ggUploadFileName);
    }
    else {
      loadShakeFile(DEFAULT_SHAKE_IMAGE); // !J! todo: move to default ..
    }

    // prime numFiles at Start
    dirCursor = 999999;// !J! grr ..
    getFileNameAtIndex(dirCursor, numFiles);
    msSystem.slog("numFiles:"); msSystem.slogln(String(numFiles));
    dirCursor = 0;// !J! grr ..


  } 

  // stop the MagicShake mode
  void stop()
  {
    lLocalImage.close();
    lPOVMode.stop();
  }

  void reset()
  {
    msSystem.slogln("MagicShake reset.");
    stop();
    start();
  }
  
  bool step()
  {

    // toggle text

    // !J! TODO: give modes an event queue ..
    if (msGlobals.ggShouldAutoLoad == 1) {
      loadShakeFile(msGlobals.ggUploadFileName);
      msGlobals.ggShouldAutoLoad = 0;
    }

    if (msSystem.msButtons.msBtnALongHit == true) {
      msSystem.msButtons.msBtnALongHit = false;
      correctBrightness = !correctBrightness;
    }
    else // either a long or a short hit .. 
    if (msSystem.msButtons.msBtnAHit == true) {
      msSystem.msButtons.msBtnAHit = false;

      dirCursor++;
      if (dirCursor >= numFiles) dirCursor = 0;

      String toLoad = getFileNameAtIndex(dirCursor, numFiles);

      // out of bounds
      if (toLoad.length() == 0) { 
        dirCursor = 0;
        toLoad = getFileNameAtIndex(0, numFiles);
        if (toLoad.length() == 0) // !J! todo: default
          toLoad = String("blueghost_png.magicBitmap");
      }

      if (toLoad.length() > 0) {
        loadShakeFile(toLoad.c_str());
      }

    }

    if (msSystem.msButtons.msBtnBHit == true) {
      msSystem.msButtons.msBtnBHit = false;

      dirCursor--;
      if (dirCursor < 0) dirCursor = numFiles - 1; // !J!

      // msSystem.slog("B cursor:"); msSystem.slogln(String(dirCursor));

      String toLoad = getFileNameAtIndex(dirCursor, numFiles);
      // msSystem.slog("Would DISP:"); msSystem.slogln(toLoad);

      // out of bounds
      if (toLoad.length() == 0) { 
        dirCursor = numFiles;
        toLoad = getFileNameAtIndex(numFiles, numFiles);
        if (toLoad.length() == 0) // !J! todo: default
          toLoad = String("blueghost_png.magicBitmap");
      }

      if (toLoad.length() > 0) {
        loadShakeFile(toLoad.c_str());
      }
    }

    // msSystem.slog("numFiles:"); msSystem.slogln(String(numFiles));

     if (lPOVMode.step()) {
        return true;
    }
    else
    {
      float fX = msGlobals.ggAccel[0];
      float fY = msGlobals.ggAccel[1];
      msModeBouncingBall.applyForce((msGlobals.ggCurrentMicros - msGlobals.ggLastMicros) / 1000.0, fX*3);
      msModeBouncingBall.simpleBouncingBall();

      delay(100);
      return false;
    }

  }

};
