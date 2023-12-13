
# Quine-McCluskey Algorithm

This C program implements the Quine-McCluskey algorithm for Boolean function minimization.

## Algorithm Details

The Quine-McCluskey algorithm implemented in this program follows these steps:

**Initialization:**
- Read the number of variables (`n`) and truth table outputs.
- Create an implicant list based on the truth table outputs.

**Combining Implicants:**
- Repeat until no more combinations are possible:
  - Combine implicants iteratively based on their values and masks.

**Generate Simplified Boolean Expression:**
- For each remaining implicant:
  - Convert the implicant to a string representation.
  - Check for duplicate terms in the result.
  - If not a duplicate, add the term to the result.

**Output the Result:**
- Print or return the simplified Boolean expression.


## Introduction

Boolean function minimization is a crucial step in digital logic design, aiding in the reduction of logical expressions to their simplest form. The Quine-McCluskey algorithm is one such method that systematically simplifies Boolean expressions, providing an efficient way to minimize logical functions.

This project offers a C implementation of the Quine-McCluskey algorithm. By providing the number of variables (`n`) and their corresponding truth table outputs, the program generates a simplified Boolean expression in sum-of-products (SOP) form.

## Pseudocode
function quine_mccluskey(n, outputs):
    all_outputs_zero = check_all_outputs_zero(outputs)
    if all_outputs_zero:
        return "0"

  implicants = initialize_implicants(n, outputs)
  combined = combine_implicants(implicants)
  
  while combined > 0:
    combined = combine_implicants(implicants) 
    
  result = generate_simplified_expression(implicants) 
  return result

function initialize_implicants(n, outputs):
    implicants = []
    for i in range(2^n):
        if outputs[i] == 1:
            insert_implicant(implicants, i, 0)
    return implicants

function combine_implicants(implicants):
    combined = 0
    for i in range(length(implicants)):
        for j in range(i + 1, length(implicants)):
            diff = implicants[i].value XOR implicants[j].value
            if count_bits(diff) == 1 and implicants[i].mask == implicants[j].mask:
                insert_implicant(implicants, implicants[i].value, implicants[i].mask | diff)
                implicants[i].used = true
                implicants[j].used = true
                combined = combined + 1
    return combined

function generate_simplified_expression(implicants):
    result = ""
    for implicant in implicants:
        if implicant.used:
            term = implicant_to_string(implicant, n)
            if not is_term_duplicate(result, term):
                if result is not empty:
                    result = result + " + "
                result = result + term
                implicant.used = true
    return result
