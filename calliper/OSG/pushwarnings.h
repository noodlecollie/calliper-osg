// This means we can treat warnings as errors without having to
// worry about OSG's various warnings (boo).

#ifdef Q_OS_MACOS
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
