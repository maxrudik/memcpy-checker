#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Frontend/CheckerRegistry.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/DynamicSize.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/SValBuilder.h"
#include <iostream>

using namespace clang;
using namespace ento;

namespace {
class MainCallChecker : public Checker<check::PreCall> {
  mutable std::unique_ptr<BugType> BT;

public:
  void checkPreCall(const CallEvent &Call, CheckerContext &C) const;
};
}

void MainCallChecker::checkPreCall(const CallEvent &Call, CheckerContext &C) const {
	
	ProgramStateRef state = C.getState();

	if (const IdentifierInfo *II = Call.getCalleeIdentifier()) {		
		if (II->isStr("memcpy")) {

			unsigned NumArgs = Call.getNumArgs();
			if (NumArgs  != 3) {
				std::cout << "Not 3 args for memcpy" << std::endl;
			}
			else {
				SVal DestSVal = Call.getArgSVal(0); // DESTENATION

				const MemRegion *P1 = DestSVal.getAsRegion();
				const ElementRegion *ER1 = dyn_cast_or_null<ElementRegion>(P1);
				
				if (!ER1) {
					return;
				}

				DefinedOrUnknownSVal DestSize = getDynamicSize(state, ER1->getSuperRegion(), C.getSValBuilder());
				
				DefinedOrUnknownSVal BufSize = Call.getArgSVal(2).castAs<DefinedOrUnknownSVal>(); // BUFSIZE				

				ProgramStateRef StateGreaterDest = state -> assumeInBound(BufSize, DestSize, true); // Symbolic value assumptions
  				ProgramStateRef StateGreaterBufSize = state -> assumeInBound(BufSize, DestSize, false);
				
				if (StateGreaterBufSize) {
					ExplodedNode *N = C.generateErrorNode();
					if (!N)
					return;

					if (!BT)
					BT.reset(new BugType(this, "Unsafe memcpy usage: amount of bytes to move can be greater then destenation size", "Max analyzer plugin"));

					auto report =
						std::make_unique<PathSensitiveBugReport>(*BT, BT->getDescription(), N);
					C.emitReport(std::move(report));
				}
			}
		}
	}
}


extern "C" void clang_registerCheckers(CheckerRegistry &registry) {
  registry.addChecker<MainCallChecker>(
      "example.MemcpyChecker", "Disallows calls to functions called memcpy",
      "");
}

extern "C" const char clang_analyzerAPIVersionString[] =
    CLANG_ANALYZER_API_VERSION_STRING;
