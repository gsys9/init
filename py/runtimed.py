import sys
import os
import subprocess
import signal
import configparser
import ctypes
import threading
import importlib.util

# Variable names that are referring to:
#       temporary variables
#       other insignificant variables
#       variables that are not constants
# often have names that, in general, seem
# meaningless, but have had some meaning,
# or may have appeared to have some far-
# fetched meaning at the time of programming,
# to the programmer. If you are editing the
# code, please do change these, and request
# that your change be merged. I will almost
# always accept such changes; I know it is
# bad programming style. These names are
# usually related to the type of the variable,
# with one meaningless letter added to the
# start or the end.
#   If these offend you particularly, by all
# means go through the entire file and change
# every single one. I will be delighted, and
# I may even apologise for this bad practice.
# (and change my ways for this and future
# programs.)
#
#   Likewise, I tend to miss out comments.
# Again, if this offends you, add one, two,
# or as many as you wish. Comment the whole
# file, if you want to. Do not necessarily
# expect that I will then comment all my code;
# when I write code, I am often in a rush
# and I just want to get it working.
#   Finally, you will notice that there are
# two languages supported. Currently the only
# translated text is the 'panic' message (if
# we are PID 1, I do not want to return
# control back to the kernel, as that usually
# generates lots of information on the screen
# that is not relevant, and often obscures
# runtimed's actual error message. Instead,
# it makes sense that runtimed prints its
# debugging information, and then a message
# that may make some sense to an end user,
# rather than lots of hexadecimal numbers
# and a message:
#   tried to kill init!
# or something similar. I did not even
# know what that meant a year ago, and
# I would not expect an end user to
# either.)


NOLOAD_EMERGENCY    = True
ARGS_TACET          = False
ID_COUNT            = 0
LOGFILE             = None
DO_WRITE_COLORS     = True
LOCALE              = "ENGLISH"
TIMEO               = 30

AUTOBOOT            = None
JOBSDIR             = None

class colors:
    # For use with common.log
    ERROR       = 31
    GOOD        = 32
    WARN        = 33
    DEFAULT     = 39

class panicmsg:
    ENGLISH = """
Your computer has encountered a fatal
error and needs to be restarted. To 
help ascertain the cause of this error,
you can read the system log.
    """

class common:
    def clear_screen():
        sys.stdout.write("\033[2J\033[H")
        sys.stdout.flush()
    def open_log():
        global LOGFILE
        try:
            LOGFILE = open("/run/runtimed.log", "w")
            common.log("log", "opened system log.")
        except:
            LOGFILE = None
            common.log("log", "logging has been disabled.", colors.WARN)
    def log(name, output, color=colors.DEFAULT, quiet=False):
        if quiet and ARGS_TACET: pass
        else: 
            newname = name + (" "*(24-len(name)))
            if DO_WRITE_COLORS:
                output1 = "\033[1;%sm%s:\033[0m %s\n"%(color, newname, output)
            else:
                output1 = "%s: %s\n"%(newname, output)

            sys.stdout.write(output1)
            if LOGFILE is not None:
                LOGFILE.write(output1)
                LOGFILE.flush()
            sys.stdout.flush()
    def load_libc():
        global libc
        common.log("ctypes", "loading libc.so")
        try:
            libc = ctypes.CDLL("libc")
        except:
            common.log(
                "ctypes", 
                "failed to load libc.so; syscalls functionality will be unavailable.", 
                colors.WARN
            )
            libc = False
    def load_ext(path):
        global ext
        global sysname
        sysname = os.uname().sysname
        extname = "ext%s"%sysname.lower()
        try:
            spec = importlib.util.spec_from_file_location("ext", "/usr/share/runtimed/ext.py")
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
        except:
            common.log(
                extname,
                "Failed to load system-specific extensions.",
                colors.WARN
            )
            return
        ext = module
    def panic():
        common.log("panic", "program lifecycle ended prematurely.", colors.ERROR)
        if os.getpid() == 1:
            print(getattr(panicmsg, LOCALE).replace("\n", ""))
        else:
            exit(7)
    def reboot(magicnum):
        if not libc:
            common.log("ctypeswrapper", "libc not available; reboot is disabled.")
        else:
            common.log("ctypeswrapper", "requesting system reboot...")
            libc.reboot(magicnum)
    def welcome_banner():
        sysinfo = os.uname()
        common.log(
            "runtimed",
            "starting %s (version %s)"%(
                sysinfo.sysname,
                sysinfo.release
            )
        )

class sighandlers:
    def register():
        signal.signal(signal.SIGCHLD, sighandlers.sigchld)
        signal.signal(signal.SIGALRM, sighandlers.sigchld)
    def sigchld():
        try:
            while os.waitpid(-1, os.WNOHANG)[0] > 0: pass
        except ChildProcessError:
            pass
        signal.alarm(TIMEO)

jobs = []

def find_job_by_name(name):
    for jobt in jobs:
        if jobt.idname == name:
            return jobt
    return None

def find_job_by_id(id):
    for jobt in jobs:
        if jobt.id == id:
            return jobt
    return None

class job:
    def __init__(
            self,
            idname,
            name, 
            description, 
            exec, 
            runlevel,
            depends,
            type,
            flags
        ):
        self.idname         = idname
        self.name           = name
        self.description    = description
        self.exec           = exec
        self.runlevel       = runlevel
        if "None" not in self.depends:
            self.depends        = depends
        else:
            self.depends        = None
        self.type           = type
        self.flags          = flags
        self.id             = ID_COUNT
        self.hasrun         = False
        ID_COUNT            += 1
    def wait(self):
        self.proc.wait()
        if self.type == "Daemon":
            if self.proc.returncode != 0:
                common.log(self.idname, "exited with error code %s."%self.proc.returncode, colors.ERROR)
            else:
                common.log(self.idname, "finished without errors.")
        elif self.type == "Task":
            if self.proc.returncode != 0:
                common.log(self.idname, "exited prematurely with error code %s."%self.proc.returncode, colors.ERROR)
            else:
                common.log(self.idname, "finished prematurely.", colors.WARN)
    def run(self, nocheckdeps=False):
        if not nocheckdeps:
            for jobtn in self.depends:
                ojob = find_job_by_name(jobtn)
                if not ojob.hasrun:
                    common.log(self.idname, "not all dependencies have been run. try again later.", colors.WARN)
                    return
        common.log(self.idname, "running job...")
        self.proc = subprocess.Popen(["/bin/sh", "-c", self.exec])
        self.prwaitthread = threading.Thread(target=self.wait)
        self.prwaitthread.start()

def add_job(dictt, cfgname):
    try:
        if find_job_by_name(dictt["Name"]) is not None:
            common.log("jobmgmt", "skipping %s, record already exists."%dictt["Name"], colors.WARN)
            return
        flags = {}
        if "+last" in dictt["Flags"]:    flags["last"] = True
        else:                            flags["last"] = False
        jobs.append(
            job(
                dictt["Name"],
                dictt["Description"],
                dictt["Exec"],
                int(dictt["Runlevel"]),
                dictt["Depends"].split(" "),
                dictt["Type"],
                flags
            )
        )
    except KeyError:
        common.log("jobmgmt", "bad format: %s"%cfgname, colors.ERROR)

def parse_master():
    common.log("datastore", "trying to load master file...", quiet=True)
    try:
        config = configparser.ConfigParser()
        config.read("/usr/share/runtimed/master.ini")
    except:
        common.log("datastore", "could not parse master file.", colors.ERROR)
        common.panic()
    for jobv in config.sections():
        if jobv != "nulltarget":
            add_job(config[jobv], "/usr/share/runtimed/master.ini")
        if jobv == "nulltarget":
            AUTOBOOT = config[jobv]["Autoboot"]
            JOBSDIR = config[jobv]["Jobs"]

def parse_cfg(filename):
    try:
        config = configparser.ConfigParser()
        config.read(filename)
    except:
        common.log("datastore", "could not parse %s"%filename, colors.WARN)
    for jobv in config.sections():
        if jobv != "nulltarget":
            add_job(config[jobv])

class init:
    def loadjobs():
        parse_master()
        files = []
        try:
            for file in os.listdir(JOBSDIR):
                files.append(JOBSDIR+"/"+files)
        except:
            common.log("jobmgmt", "could not access %s"%JOBSDIR)
            return
        for file in files:
            parse_cfg(file)
    def getorder(jobids):
        tmpjobids = jobids.copy()
        array = []
        curpartofarray = []
        for jobid in jobids:
            ijob = find_job_by_id(jobid)
            if ijob.depends == None:
                curpartofarray.append(jobid)
                tmpjobids.remove(jobid)
                continue
            for dep in ijob.depends:
                hasfound = False
                for jobid2 in jobids:
                    ijob2 = find_job_by_id(jobid2)
                    if ijob2.idname == dep:
                        hasfound = True
                        break
                if hasfound == False:
                    common.log("jobmgmt", "job %s depends on %s which does not exist. skipping."%(ijob.idname, dep), colors.WARN)
                    tmpjobids.remove(jobid)
        array.append(curpartofarray)
        curpartofarray.clear()
        def gensection():
            for jobid in jobids:
                ijob = find_job_by_id(jobid)
                count = len(ijob.depends)
                count1 = 0
                for item in ijob.depends:
                    dpjob = find_job_by_name(item).id
                    if dpjob.id not in tmpjobids:
                        count1 += 1
                if count == count1:
                    curpartofarray.append(jobid)
                    tmpjobids.remove(jobid)
            array.append(curpartofarray)
            curpartofarray.clear()
        while len(tmpjobids) != 0:
            gensection()
        return array
    def execorder(order):
        count = 1
        for section in order:
            common.log("init", "startup, stage %s"%count)
            proclist = []
            for jobd in section:
                jobt = find_job_by_id(jobd)
                jobt.run(nocheckdeps=True)
                proclist.append(jobt.proc)
            for proc in proclist:
                proc.wait()
            count += 1

    def main(argv):
        common.clear_screen()
        common.log("runtimed", "handoff complete.", quiet=True)
        common.log("runtimed", "recieved args %s"%argv, quiet=True)
        common.welcome_banner()
        common.load_libc()
        sighandlers.register()
        if NOLOAD_EMERGENCY:
            common.log("init", "requesting root login for debug...", colors.WARN)
            subprocess.run(["/sbin/sulogin"])
        init.loadjobs()
        order = init.getorder(range(0, ID_COUNT-1))
        init.execorder(order)

        while True: pass


def main(argv):
    if os.getpid() == 1:
        common.log("runtimed", "we are PID 1, initialising...", colors.GOOD)
        init.main(argv)
    elif len(argv) == 1:
        print("""
Options are required unless we are PID 1.
Usage:  
        runtimed COMMAND
        runtimed [RUNLEVEL]
        init [RUNLEVEL]
        bootstrapinit [RUNLEVEL]
        COMMAND [OPTIONS]
              
    Invokes COMMAND with [OPTIONS], or, as
    PID 1, runs as init [RUNLEVEL].
""")
        exit(1)
    elif argv[1] == "init":
        common.log("init", "we are not PID 1. skipping initialisation.")
        common.panic()

if __name__ == "__main__":
    exit(main(sys.argv))
