licenses(["notice"])

exports_files([
  "LICENSE",
  "NOTICE",
])

COPTS = [
  "-UNDEBUG",
]

LIBS = []

cc_library(
  name = "factory",
  srcs = glob(
    ["src/**/*.cc"],
    exclude = ["src/**/*_TEST*"],
  ),
  hdrs = glob(
    [
      "src/**/*.h",
      "src/**/*.tcc",
    ],
    exclude = ["src/**/*_TEST*"],
  ),
  copts = COPTS,
  deps = LIBS,
  includes = [
    "src",
  ],
  visibility = ["//visibility:public"],
  alwayslink = 1,
)

cc_library(
  name = "test_lib",
  srcs = glob([
    "src/**/*_TEST*.cc"
  ]),
  hdrs = glob([
    "src/**/*_TEST*.h",
    "src/**/*_TEST*.tcc",
  ]),
  copts = COPTS,
  deps = [
    ":factory",
    "@googletest//:gtest_main",
  ] + LIBS,
  visibility = ["//visibility:private"],
  alwayslink = 1,
)

cc_test(
  name = "factory_test",
  copts = COPTS,
  deps = [
    ":test_lib",
  ] + LIBS,
  args = [
    "--gtest_color=yes",
  ],
  visibility = ["//visibility:public"],
)

genrule(
  name = "lint",
  srcs = glob([
    "src/**/*.cc"
  ]) + glob([
    "src/**/*.h",
    "src/**/*.tcc",
  ]),
  outs = ["linted"],
  cmd = """
    python $(location @cpplint//:cpplint) \
      --root=$$(pwd)/src \
      --headers=h,tcc \
      --extensions=cc,h,tcc \
      --quiet $(SRCS) > $@
    echo // $$(date) > $@
  """,
  tools = [
    "@cpplint//:cpplint",
  ],
  visibility = ["//visibility:public"],
)
