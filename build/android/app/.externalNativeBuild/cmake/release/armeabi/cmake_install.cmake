INDX( 	 ��KVT           (   8   �       ~ E_NS                              �]   6 ���&kv��S�&kv����&kv����&kv��      �              T a r g e t D i r e c t o r i e s . t x t                  �]   6 ���&kv��S�&kv����&kv����&kv��      �              T A R G E T ~ 1 . T X T . T M               �]   6 ���&kv��S�&kv����&kv����&kv��      �              T A R G E T ~ 1 . T X T . T M               �]   6 ���&kv��S�&kv����&kv����&kv��      �              T A R G E T  1 . T X T . T M               p Z     �]   6 ���&kv��S�&kv����&kv����&kv��      �              T A R G E T ~ 1 . T X T . T M               ���&kv��S�&kv����&kv����&kv��      �              T A R G E T ~ 1 . T X T . T M                      message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAK INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/Joel/Desktop/UAE2dEngine-repository/build/android/app/.externalNativeBuild/cmake/release/armeabi/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_I TALL_MANIFEST_CONTENT}")
