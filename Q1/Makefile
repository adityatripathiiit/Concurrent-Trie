CC      := gcc
CCFLAGS := -Wall -Werror -pthread -g

DEPS := trie.h trie.c
B_TARGETS := test_trie_single_threaded test_trie_s_lock test_trie_rw_lock test_trie_hoh_lock
R_TARGETS := single_threaded s_lock rw_lock hoh_lock
ST_TARGETS := single_threaded
MT_TARGETS := s_lock rw_lock hoh_lock

test_all: $(R_TARGETS)

test_trie_rw_lock.c: test_trie_s_lock.c
	@tail -n +2 $< > $@
	@echo Updated $@

test_trie_hoh_lock.c: test_trie_s_lock.c
	@tail -n +3 $< > $@
	@echo Updated $@

$(B_TARGETS): % : %.c $(DEPS)
	@echo Building
	$(CC) -o $@ $^ $(CCFLAGS)

$(ST_TARGETS): % : test_trie_%
	@rm -f -r ./.testout
	@mkdir .testout
	@echo "\n-------------------------------------------"
	@echo "Program Output($<):"
	@echo "-------------------------------------------"
	@-valgrind --leak-check=full --show-leak-kinds=all --log-file="./.testout/memtest_out_$<" ./$<
	@echo "\n-------------------------------------------"
	@echo "Valgrind Output($<):"
	@echo "-------------------------------------------"
	@cat ./.testout/memtest_out_$<

$(MT_TARGETS): % : test_trie_%
	@rm -f -r ./.testout
	@mkdir .testout
	@echo "\n-------------------------------------------"
	@echo "Program Output($<):"
	@echo "-------------------------------------------"
	@-valgrind --leak-check=full --show-leak-kinds=all --log-file="./.testout/memtest_out_$<" ./$<
	@echo "\n-------------------------------------------"
	@echo "Valgrind Memcheck Output($<):"
	@echo "-------------------------------------------"
	@cat ./.testout/memtest_out_$<
	@-valgrind --tool=helgrind --log-file="./.testout/heltest_out_$<" ./$< > /dev/null
	@echo "\n-------------------------------------------"
	@echo "Valgrind Helgrind Output($<):"
	@echo "-------------------------------------------"
	@cat ./.testout/heltest_out_$<

clean: 
	rm -f $(B_TARGETS)
	rm -f -r ./.testout
	rm -f -r ./test_trie_rw_lock.c
	rm -f -r ./test_trie_hoh_lock.c

# Add your rules after this point. Don't edit the things above:
