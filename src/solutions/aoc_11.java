import java.math.BigInteger;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class JavaTest
{

    static HashMap<Map<BigInteger, Integer>, BigInteger> cache = new HashMap<>();

    public static void main(String[] args)
    {
        try
        {
            String input = new String(Files.readAllBytes(Paths.get("./resources/aoc_data/aoc_11_data.txt")));
            String[] inputSplit = input.replace("\n", "").split(" ");

            List<BigInteger> nums = Arrays.stream(inputSplit).map(i -> new BigInteger(i)).toList();

            System.out.println(nums.toString());

            int depth = 75;
            BigInteger result = BigInteger.valueOf(0);
            for (int i = 0; i < nums.size(); i++) {
                result = result.add(ansRec(nums.get(i), depth));
            }

            System.out.println("RESULT: " + result);
        }
        catch (Exception e)
        {
            System.out.println(e.getMessage());
        }
    }

    public static BigInteger ansRec(BigInteger stone, int depth)
    {
        if (depth < 1)
            return BigInteger.valueOf(1);

        Map<BigInteger, Integer> key = new HashMap<>();
        key.put(stone, depth);

        if (cache.get(key) != null)
            return cache.get(key);

        BigInteger retval = null;
        int n_digits = stone.toString().length();

        if (stone.compareTo(BigInteger.valueOf(0)) == 0) {
            retval = ansRec(BigInteger.valueOf(1), depth - 1);
        }

        else if (n_digits % 2 == 1) {
            retval = ansRec(stone.multiply(BigInteger.valueOf(2024)), depth - 1);
        }

        else if (n_digits % 2 == 0) {
            BigInteger l = new BigInteger((stone.toString().substring(0, n_digits / 2)));
            BigInteger r = new BigInteger((stone.toString().substring(n_digits / 2)));
            retval = ansRec(r, depth - 1).add(ansRec(l, depth - 1));
        }

        cache.put(key, retval);
        return retval;
    }

}

