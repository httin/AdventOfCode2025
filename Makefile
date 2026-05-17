CXX := g++
CXXFLAGS := -std=c++17 -O2

.PHONY: FORCE clean create

day%: day%.cpp FORCE
	$(CXX) $(CXXFLAGS) -o $@ $<
	./$@

create:
ifndef DAY
	$(error Usage: make create DAY=<N>)
endif
	@if [ -f "day$(DAY).cpp" ]; then echo "day$(DAY).cpp already exists"; exit 1; fi
	@sed 's/DAY_N/$(DAY)/g' template.cpp > day$(DAY).cpp
	@touch day$(DAY).txt
	@echo "Created day$(DAY).cpp and day$(DAY).txt"

clean:
	rm -f day1 day2 day3 day4 day5 day6 day7 day8 day9 day10 \
	      day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 \
	      day21 day22 day23 day24 day25