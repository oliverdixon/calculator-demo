import java.util.Deque;
import java.util.ArrayDeque;

class Calculator {
        public static void main(String[] args) {
                final int TEST_COUNT = 32;
                Deque<Integer> stack = new ArrayDeque<Integer>();

                for ( int i = 0; i < TEST_COUNT; i++ )
                        stack.push(i);

                System.out.println(stack);
        }
}

