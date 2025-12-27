#!/usr/bin/env python3
# trunk-ignore-all(ruff/F821)
# trunk-ignore-all(flake8/F821): For SConstruct imports
import shutil

Import("env")
Import("projenv")

ccache_path = "ccache"

def check_ccache():
    return shutil.which(ccache_path) is not None

def inject_ccache(target_env):
    _as = target_env.get("AS")
    if _as and not _as.startswith(ccache_path):
        target_env.Replace(AS=f"{ccache_path} {_as}")
    cc = target_env.get("CC")
    if cc and not cc.startswith(ccache_path):
        target_env["CC"] = f"{ccache_path} {cc}"
    cxx = target_env.get("CXX")
    if cxx and not cxx.startswith(ccache_path):
        target_env.Replace(CXX=f"{ccache_path} {cxx}")
    # TODO: cache linker too

if check_ccache():
    print("ccache found, starting injection...")
    inject_ccache(env)
    for builder in env.GetLibBuilders():
        inject_ccache(builder.env)
    inject_ccache(projenv)
else:
    print("ccache not found, skipping injection.")