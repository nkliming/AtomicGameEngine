/*
    SDL_android_main.c, placed in the public domain by Sam Lantinga  3/13/14
*/

// Modified by Lasse Oorni and Yao Wei Tjong for Urho3D

// Urho3D - use SDK include dir
// ATOMIC BEGIN

// We don't "install" the Third Party sources, so use from build tree
#include <SDL/src/SDL_internal.h>

// ATOMIC END

#ifdef __ANDROID__

/* Include the SDL main definition header */ // Urho3D - use SDK include dir

// ATOMIC BEGIN
#include <SDL/include/SDL_main.h>
// ATOMIC END

/*******************************************************************************
                 Functions called by JNI
*******************************************************************************/
#include <jni.h>

// Urho3D: added extra filesDir parameter
/* Called before SDL_main() to initialize JNI bindings in SDL library */
extern void SDL_Android_Init(JNIEnv* env, jclass cls, jstring filesDir);

/* This prototype is needed to prevent a warning about the missing prototype for global function below */
JNIEXPORT int JNICALL Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject array, jstring filesDir);

/* Start up the SDL app */
JNIEXPORT int JNICALL Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject array, jstring filesDir)
{
    int i;
    int argc;
    int status;
    int len;
    char** argv;

    /* This interface could expand with ABI negotiation, callbacks, etc. */
    SDL_Android_Init(env, cls, filesDir);

    SDL_SetMainReady();

    /* Prepare the arguments. */

    len = (*env)->GetArrayLength(env, array);
    argv = SDL_stack_alloc(char*, len + 1);
    argc = 0;
    // Urho3D: avoid hard-coding the "app_process" as the first argument
    for (i = 0; i < len; ++i) {
        const char* utf;
        char* arg = NULL;
        jstring string = (*env)->GetObjectArrayElement(env, array, i);
        if (string) {
            utf = (*env)->GetStringUTFChars(env, string, 0);
            if (utf) {
                arg = SDL_strdup(utf);
                (*env)->ReleaseStringUTFChars(env, string, utf);
            }
            (*env)->DeleteLocalRef(env, string);
        }
        if (!arg) {
            arg = SDL_strdup("");
        }
        argv[argc++] = arg;
    }
    argv[argc] = NULL;


    /* Run the application. */

    status = SDL_main(argc, argv);

    /* Release the arguments. */

    for (i = 0; i < argc; ++i) {
        SDL_free(argv[i]);
    }
    SDL_stack_free(argv);
    /* Do not issue an exit or the whole application will terminate instead of just the SDL thread */
    /* exit(status); */

    return status;
}

#endif /* __ANDROID__ */

/* vi: set ts=4 sw=4 expandtab: */
