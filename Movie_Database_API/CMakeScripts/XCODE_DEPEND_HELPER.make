# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.main.Debug:
PostBuild.src.Debug: /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/Debug/main
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/Debug/main:\
	/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Debug/libsrc.a\
	/usr/local/lib/libcpprest.dylib\
	/usr/local/lib/libboost_random.dylib\
	/usr/local/lib/libboost_system.dylib\
	/usr/local/lib/libboost_thread-mt.dylib\
	/usr/local/lib/libboost_filesystem.dylib\
	/usr/local/lib/libboost_chrono.dylib\
	/usr/local/lib/libboost_atomic-mt.dylib\
	/usr/local/lib/libboost_date_time.dylib\
	/usr/local/lib/libboost_regex.dylib\
	/usr/local/opt/openssl/lib/libssl.dylib\
	/usr/local/opt/openssl/lib/libcrypto.dylib
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/Debug/main


PostBuild.src.Debug:
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Debug/libsrc.a:
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Debug/libsrc.a


PostBuild.tests.Debug:
PostBuild.src.Debug: /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/Debug/tests
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/Debug/tests:\
	/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Debug/libsrc.a\
	/usr/local/lib/libcpprest.dylib\
	/usr/local/lib/libboost_random.dylib\
	/usr/local/lib/libboost_system.dylib\
	/usr/local/lib/libboost_thread-mt.dylib\
	/usr/local/lib/libboost_filesystem.dylib\
	/usr/local/lib/libboost_chrono.dylib\
	/usr/local/lib/libboost_atomic-mt.dylib\
	/usr/local/lib/libboost_date_time.dylib\
	/usr/local/lib/libboost_regex.dylib\
	/usr/local/opt/openssl/lib/libssl.dylib\
	/usr/local/opt/openssl/lib/libcrypto.dylib
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/Debug/tests


PostBuild.main.Release:
PostBuild.src.Release: /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/Release/main
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/Release/main:\
	/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Release/libsrc.a\
	/usr/local/lib/libcpprest.dylib\
	/usr/local/lib/libboost_random.dylib\
	/usr/local/lib/libboost_system.dylib\
	/usr/local/lib/libboost_thread-mt.dylib\
	/usr/local/lib/libboost_filesystem.dylib\
	/usr/local/lib/libboost_chrono.dylib\
	/usr/local/lib/libboost_atomic-mt.dylib\
	/usr/local/lib/libboost_date_time.dylib\
	/usr/local/lib/libboost_regex.dylib\
	/usr/local/opt/openssl/lib/libssl.dylib\
	/usr/local/opt/openssl/lib/libcrypto.dylib
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/Release/main


PostBuild.src.Release:
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Release/libsrc.a:
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Release/libsrc.a


PostBuild.tests.Release:
PostBuild.src.Release: /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/Release/tests
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/Release/tests:\
	/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Release/libsrc.a\
	/usr/local/lib/libcpprest.dylib\
	/usr/local/lib/libboost_random.dylib\
	/usr/local/lib/libboost_system.dylib\
	/usr/local/lib/libboost_thread-mt.dylib\
	/usr/local/lib/libboost_filesystem.dylib\
	/usr/local/lib/libboost_chrono.dylib\
	/usr/local/lib/libboost_atomic-mt.dylib\
	/usr/local/lib/libboost_date_time.dylib\
	/usr/local/lib/libboost_regex.dylib\
	/usr/local/opt/openssl/lib/libssl.dylib\
	/usr/local/opt/openssl/lib/libcrypto.dylib
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/Release/tests


PostBuild.main.MinSizeRel:
PostBuild.src.MinSizeRel: /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/MinSizeRel/main
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/MinSizeRel/main:\
	/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/MinSizeRel/libsrc.a\
	/usr/local/lib/libcpprest.dylib\
	/usr/local/lib/libboost_random.dylib\
	/usr/local/lib/libboost_system.dylib\
	/usr/local/lib/libboost_thread-mt.dylib\
	/usr/local/lib/libboost_filesystem.dylib\
	/usr/local/lib/libboost_chrono.dylib\
	/usr/local/lib/libboost_atomic-mt.dylib\
	/usr/local/lib/libboost_date_time.dylib\
	/usr/local/lib/libboost_regex.dylib\
	/usr/local/opt/openssl/lib/libssl.dylib\
	/usr/local/opt/openssl/lib/libcrypto.dylib
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/MinSizeRel/main


PostBuild.src.MinSizeRel:
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/MinSizeRel/libsrc.a:
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/MinSizeRel/libsrc.a


PostBuild.tests.MinSizeRel:
PostBuild.src.MinSizeRel: /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/MinSizeRel/tests
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/MinSizeRel/tests:\
	/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/MinSizeRel/libsrc.a\
	/usr/local/lib/libcpprest.dylib\
	/usr/local/lib/libboost_random.dylib\
	/usr/local/lib/libboost_system.dylib\
	/usr/local/lib/libboost_thread-mt.dylib\
	/usr/local/lib/libboost_filesystem.dylib\
	/usr/local/lib/libboost_chrono.dylib\
	/usr/local/lib/libboost_atomic-mt.dylib\
	/usr/local/lib/libboost_date_time.dylib\
	/usr/local/lib/libboost_regex.dylib\
	/usr/local/opt/openssl/lib/libssl.dylib\
	/usr/local/opt/openssl/lib/libcrypto.dylib
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/MinSizeRel/tests


PostBuild.main.RelWithDebInfo:
PostBuild.src.RelWithDebInfo: /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/RelWithDebInfo/main
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/RelWithDebInfo/main:\
	/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/RelWithDebInfo/libsrc.a\
	/usr/local/lib/libcpprest.dylib\
	/usr/local/lib/libboost_random.dylib\
	/usr/local/lib/libboost_system.dylib\
	/usr/local/lib/libboost_thread-mt.dylib\
	/usr/local/lib/libboost_filesystem.dylib\
	/usr/local/lib/libboost_chrono.dylib\
	/usr/local/lib/libboost_atomic-mt.dylib\
	/usr/local/lib/libboost_date_time.dylib\
	/usr/local/lib/libboost_regex.dylib\
	/usr/local/opt/openssl/lib/libssl.dylib\
	/usr/local/opt/openssl/lib/libcrypto.dylib
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/RelWithDebInfo/main


PostBuild.src.RelWithDebInfo:
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/RelWithDebInfo/libsrc.a:
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/RelWithDebInfo/libsrc.a


PostBuild.tests.RelWithDebInfo:
PostBuild.src.RelWithDebInfo: /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/RelWithDebInfo/tests
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/RelWithDebInfo/tests:\
	/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/RelWithDebInfo/libsrc.a\
	/usr/local/lib/libcpprest.dylib\
	/usr/local/lib/libboost_random.dylib\
	/usr/local/lib/libboost_system.dylib\
	/usr/local/lib/libboost_thread-mt.dylib\
	/usr/local/lib/libboost_filesystem.dylib\
	/usr/local/lib/libboost_chrono.dylib\
	/usr/local/lib/libboost_atomic-mt.dylib\
	/usr/local/lib/libboost_date_time.dylib\
	/usr/local/lib/libboost_regex.dylib\
	/usr/local/opt/openssl/lib/libssl.dylib\
	/usr/local/opt/openssl/lib/libcrypto.dylib
	/bin/rm -f /Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/tests/RelWithDebInfo/tests




# For each target create a dummy ruleso the target does not have to exist
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Debug/libsrc.a:
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/MinSizeRel/libsrc.a:
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/RelWithDebInfo/libsrc.a:
/Users/aronvischjager/Documents/Advanced_C++/pa7-aronvis/src/Release/libsrc.a:
/usr/local/lib/libboost_atomic-mt.dylib:
/usr/local/lib/libboost_chrono.dylib:
/usr/local/lib/libboost_date_time.dylib:
/usr/local/lib/libboost_filesystem.dylib:
/usr/local/lib/libboost_random.dylib:
/usr/local/lib/libboost_regex.dylib:
/usr/local/lib/libboost_system.dylib:
/usr/local/lib/libboost_thread-mt.dylib:
/usr/local/lib/libcpprest.dylib:
/usr/local/opt/openssl/lib/libcrypto.dylib:
/usr/local/opt/openssl/lib/libssl.dylib:
