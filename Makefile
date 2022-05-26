#
# OMNeT++/OMNEST Makefile for simulation-examples
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -KINET4_4_PROJ=/home/simulator/workspace/inet4.4 -KSIMU5G_PROJ=../Simu5G -KVEINS_INET_PROJ=/home/simulator/workspace/veins_inet -KVEINS_PROJ=/home/simulator/workspace/veins -DINET_IMPORT -DVEINS_INET_IMPORT -I. -I$$\(INET4_4_PROJ\)/src -I$$\(SIMU5G_PROJ\)/src -I$$\(VEINS_INET_PROJ\)/src -I$$\(VEINS_PROJ\)/src -Isrc -L$$\(INET4_4_PROJ\)/src -L$$\(SIMU5G_PROJ\)/src -L$$\(VEINS_INET_PROJ\)/src -lINET$$\(D\) -lsimu5g$$\(D\) -lveins_inet$$\(D\)
#

# Name of target to be created (-o option)
TARGET_DIR = .
TARGET_NAME = simulation-examples$(D)
TARGET = $(TARGET_NAME)$(EXE_SUFFIX)
TARGET_IMPLIB = $(TARGET_NAME)$(IMPLIB_SUFFIX)
TARGET_IMPDEF = $(TARGET_NAME)$(IMPDEF_SUFFIX)
TARGET_FILES = $(TARGET_DIR)/$(TARGET)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -I$(INET4_4_PROJ)/src \
    -I$(SIMU5G_PROJ)/src \
    -I$(VEINS_INET_PROJ)/src \
    -I$(VEINS_PROJ)/src \
    -Isrc

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = $(LDFLAG_LIBPATH)$(INET4_4_PROJ)/src $(LDFLAG_LIBPATH)$(SIMU5G_PROJ)/src $(LDFLAG_LIBPATH)$(VEINS_INET_PROJ)/src  -lINET$(D) -lsimu5g$(D) -lveins_inet$(D)

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/src/app/AppForAddressResolving/AddressResolvingApp.o \
    $O/src/app/MobilityTester/MobilityTester.o \
    $O/src/app/MobilityTester/TriggerAMS.o \
    $O/src/app/UEAppInteractionTester/UEAppInteractionTester.o \
    $O/src/app/fakeregistrationvim/FakeRegistrationVim.o \
    $O/src/app/instantiationmecpm/InstantiationMecpm.o \
    $O/src/app/orchestratorTesters/MEOTester.o \
    $O/src/app/registrationvim/RegistrationVim.o \
    $O/src/nodes/nodeA/NodeA.o \
    $O/src/nodes/nodeB/NodeB.o \
    $O/src/msg/RegistrationPacket_m.o

# Message files
MSGFILES = \
    src/msg/RegistrationPacket.msg

# SM files
SMFILES =

# Other makefile variables (-K)
INET4_4_PROJ=/home/simulator/workspace/inet4.4
SIMU5G_PROJ=../Simu5G
VEINS_INET_PROJ=/home/simulator/workspace/veins_inet
VEINS_PROJ=/home/simulator/workspace/veins

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
CONFIGFILE = $(shell opp_configfilepath)
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)
ifneq ($(PLATFORM),win32)
LIBS += -Wl,-rpath,$(abspath $(INET4_4_PROJ)/src) -Wl,-rpath,$(abspath $(SIMU5G_PROJ)/src) -Wl,-rpath,$(abspath $(VEINS_INET_PROJ)/src)
endif

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DINET_IMPORT -DVEINS_INET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE (if COPTS has changed since last build)
# and make the object files depend on it
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
  $(shell $(MKPATH) "$O")
  $(file >$(COPTS_FILE),$(COPTS))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
-include makefrag

#------------------------------------------------------------------------------

# Main target
all: $(TARGET_FILES)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clang-msabi)
	-$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib) 2>/dev/null

$O/$(TARGET_NAME).pdb: $O/$(TARGET)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_FILES)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(CLEANALL_COMMAND)
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

help:
	@echo "$$HELP_SYNOPSYS"
	@echo "$$HELP_TARGETS"
	@echo "$$HELP_VARIABLES"
	@echo "$$HELP_EXAMPLES"

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)
