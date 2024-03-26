# The Qt Simple File Hashing Application

![image](https://github.com/ZekeDragon/SimpleFileHash/assets/135618952/9193605a-73e2-437a-9eae-57e1827a6458)

The Simple Qt File Hashing Application is an application that allows you to quickly and easily hash hundreds or more files utilizing the full power of your PC's processor cores. This application was designed from the start to be multi-threaded, supports dark mode, and has dozens of hash functions to choose from.

This application was initially designed as a weekend project, but became much larger than that as a single weekend was not sufficient to make this application presentable. Actually, even a month is cutting it close with this application, as the scope was considerably larger, but I prefer utilizing my skills on larger, more advanced projects anyway. The project's main GitHub repository should be where the most recent release can be found as well. Releases are **not** provided on KirHut.com due to bandwidth constraints.

**NOTE:** This application uses a custom MD5 implementation written in C++17. You should never count on MD5 for security or collision resistance in the first place, but moreover, do not rely on this implementation of MD5 to be 100% correct or resistant to side-channel attacks. Please report any inconsistency in the MD5 output as a bug for this project. All other hash functions are implemented by 3rd party libraries, namely Crypto++, but also for MD6 support this project uses the reference implementation published by Ron Rivest.

## Basic Usage
![image](https://github.com/ZekeDragon/SimpleFileHash/assets/135618952/c52cc69a-72d3-421f-bae9-4d5ac27f69bd)

You should be able to open the application and select files to hash by using either the **Select Files...** button or the **Select Directory...** button. You may also drag and drop files into the application and that will work just as well. The algorithm that will be used to hash the files is under the drop-down menu labeled "Hashing Algorithm." You may change this at any time to use a different hashing function.

Press the **Start Hashing!** button to begin the hashes. This will take advantage of all of the processor cores on your device to perform file hashes. Each file will have its own loading bar indicating how much progress has been done for each hash, and will give you the completed hash upon finishing the task. You may of course copy the hashes from the table using Ctrl+C. You may also cancel the hashing job if a single hash is just taking too long or you need to stop for any reason.

![image](https://github.com/ZekeDragon/SimpleFileHash/assets/135618952/4323c261-34a1-46ad-ad66-9079261813e8)

You can use the Matching Window found under the Matching menu on the main window or, after you have completed a set of hashes, by pressing the **Compare Hashes!** button. The Matching Window allows you to quickly compare hashes to files that are hashed on the main hashing window. The matches will be updated in real time when each new hashed file is completed. You may use a hash sum file (like those created by md5sum or shasum command line tools) or use a HashDeep file as a source for matching files to.

![image](https://github.com/ZekeDragon/SimpleFileHash/assets/135618952/1389742a-8f3a-4081-abce-d8454671ac77)

There are two tabs in the Matching Window: Matches Found and Unmatched Hashes. The Matches Found tab will have all of the files with hashes that match those that are being searched for, and any files that do not match any of the hashes will be put in the Unmatched Hashes tab. You may use the **Save hash matches between successive hash jobs?** checkbox to save matched and unmatched hashes between different sets of hashed files, since by default it will clear these matches when you open new files in the matching window. You may also add hashes manually to the Matching Window, though it is usually more useful to start with a hash sum/HashDeep file or use another folder of files to compare to.

![image](https://github.com/ZekeDragon/SimpleFileHash/assets/135618952/be3e5039-c086-436e-b37a-7cfa3f4a21b8)

The User Preferences dialog allows you to set preferences, such as going through subdirectories when you hash a directory, displaying in uppercase, capping the number of files that can be hashed in a directory hash job, and what the default algorithm should be. It also allows you to override the system dark or light mode settings and override the system language if you want to change those. They will instantly change in real time when the application is running, so be forewarned!

## What Remains
There is still a lot that needs to be implemented, such as:

- Getting Context Menu installation to work so users can hash files with only a right-click!
- Exporting hash sum and HashDeep files needs to be included in this software so that hashes from files can be saved. There should be a file format that SimpleFileHash uses on its own, as well.
- The command line interface still needs to be implemented, which is necessary for the context menu installers as well.
- Pausing and unpausing.
- This application needs to support hash scheduling like QuickHash-GUI does so that it can compete.

There is also a slough of bugs most likely, as this is still alpha software.
