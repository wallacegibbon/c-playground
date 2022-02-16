#include <stdio.h>

int main(int argc, const char **argv) {
	printf("0xffffffff >> 5 = %08x\n", 0xffffffff >> 5);
	// will print: 0xffffffff >> 5 = 07ffffff

	int a = 0xffffffff;
	printf("a >> 5 = %08x\n", a >> 5);
	// will print: a >> 5 = ffffffff

	return 0;
}

/*
int main(int argc, const char **argv) {
   0:	7179                	addi	sp,sp,-48
   2:	f406                	sd	ra,40(sp)
   4:	f022                	sd	s0,32(sp)
   6:	1800                	addi	s0,sp,48
   8:	87aa                	mv	a5,a0
   a:	fcb43823          	sd	a1,-48(s0)
   e:	fcf42e23          	sw	a5,-36(s0)
	printf("0xffffffff >> 5 = %08x\n", 0xffffffff >> 5);
  12:	080007b7          	lui	a5,0x8000
  16:	fff78593          	addi	a1,a5,-1 # 7ffffff <.LASF2+0x7fffee6>
  1a:	000007b7          	lui	a5,0x0
  1e:	00078513          	mv	a0,a5
  22:	00000097          	auipc	ra,0x0
  26:	000080e7          	jalr	ra # 22 <main+0x22>
	// will print: 0xffffffff >> 5 = 07ffffff

	int a = 0xffffffff;
  2a:	57fd                	li	a5,-1
  2c:	fef42623          	sw	a5,-20(s0)
	printf("a >> 5 = %08x\n", a >> 5);
  30:	fec42783          	lw	a5,-20(s0)
  34:	4057d79b          	sraiw	a5,a5,0x5
  38:	2781                	sext.w	a5,a5
  3a:	85be                	mv	a1,a5
  3c:	000007b7          	lui	a5,0x0
  40:	00078513          	mv	a0,a5
  44:	00000097          	auipc	ra,0x0
  48:	000080e7          	jalr	ra # 44 <main+0x44>
	// will print: a >> 5 = ffffffff

	return 0;
  4c:	4781                	li	a5,0
}
  4e:	853e                	mv	a0,a5
  50:	70a2                	ld	ra,40(sp)
  52:	7402                	ld	s0,32(sp)
  54:	6145                	addi	sp,sp,48
  56:	8082                	ret

*/
