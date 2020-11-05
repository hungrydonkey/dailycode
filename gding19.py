import math
from poly import *
import random
import helper
from ctypes import *
import cProfile
import time
from itertools import repeat
from multiprocessing import Pool

# https://eprint.iacr.org/2019/665.pdf
class Ding19(object):
    def __init__(self, n, q, sigma):
        self.n = n
        self.q = q
        self.sigma = sigma
        self.a = Poly.uniform(self.n, self.q)
        self.si = Ding19.gen_secret(self.n, self.q, self.sigma)
        self.ei = Ding19.gen_pubkey_error(self.n, self.q, self.sigma)
        self.sj = Ding19.gen_secret(self.n, self.q, self.sigma)
        self.ej = Ding19.gen_pubkey_error(self.n, self.q, self.sigma)

    @staticmethod
    def gen_secret(n, q, sigma):
        return Poly.discretegaussian(n, q, sigma, 1)

    @staticmethod
    def gen_pubkey_error(n, q, sigma):
        return Poly.discretegaussian(n, q, sigma)

    @staticmethod
    def gen_shared_error(n, q, sigma):
        return Poly.discretegaussian(n, q, sigma)

    @staticmethod
    def sig(q, v):
        if v > round(q / 4.0) and v < q - math.floor(q / 4):
            return 1
        else:
            return 0

    def signal(self, v):
        return [Ding19.sig(self.q, v[i]) for i in range(self.n)]

    def mod2(self, v, w):
        if self.n != v.n or self.n != len(w): raise Exception
        r = [0 for i in range(self.n)]
        for i in range(self.n):
            r[i] = int(v[i] + w[i] * (self.q - 1) / 2)
            r[i] %= self.q
            r[i] %= 2
        return r
        
    def hash1(self, Pa):
        seed = hash(str(Pa))
        return Poly.discretegaussian(self.n, self.q, self.sigma, seed)
        
    def hash2(self, xs):
        seed = hash(str(xs))
        return Poly.discretegaussian(self.n, self.q, self.sigma, seed)

    def alice_init(self):
        self.xi = self.a * self.si + 2 * self.ei
        return self.xi

    #def bob(self, Pi, Pj, xi, simplified=True):
    def bob(self, Pa):
        #self.yj = self.a * self.sj + 2 * self.ej
        es = Ding19.gen_pubkey_error(self.n, self.q, self.sigma)
        rs = Ding19.gen_secret(self.n, self.q, self.sigma)
        self.xs = self.a * rs + 2 * es  
        #print(self.xs)
        #c = self.hash1(Pi, Pj, xi)
        #d = self.hash2(Pi, Pj, xi, self.yj)
        c = self.hash1(Pa)
        d = self.hash2(self.xs)
        
       
        self.eb = Ding19.gen_shared_error(self.n, self.q, self.sigma)

        Pabar = Pa + self.a * c 
        self.kb = Pabar * (self.sj + d) + 2 * self.eb
    
        self.wj = self.signal(self.kb)
        self.skj = self.mod2(self.kb, self.wj)
        return (self.xs, self.wj, self.skj)
        
''' 
    def alice_resp(self, Pi, Pj, yj, wj):
        c = self.hash1(Pi, Pj, self.xi)
        d = self.hash2(Pi, Pj, self.xi, yj)
        self.fi = Ding19.gen_shared_error(self.n, self.q, self.sigma)
        yjbar = yj + self.a * d + 2 * self.fi
        self.gi = Ding19.gen_shared_error(self.n, self.q, self.sigma)
        self.ki = yjbar * (self.si + c) + 2 * self.gi
        self.ski = self.mod2(self.ki, wj)
        return self.ski
'''
def get_zeros(coeffs, n):
    zero_count = 0
    temp_count = 0
    for i in range(n):
        if coeffs[i] == 0:
            temp_count += 1
        else:
            if temp_count > zero_count:
                zero_count = temp_count
            temp_count = 0
    return zero_count

def process_k(k, n, kn_bnd, t):
    global execution
    global f
    #Pj = "Bob"
    #Pi = 0
    signals = [None for i in range(n)]
    a = execution.a
    e = ((k+1)*t) * f
    got_signals = 0
    while got_signals < n:
        #Pi = Pi + 1
        #print(got_signals)
        (xs, wj, skj) = execution.bob(e)
        c = execution.hash1(e)
        d = execution.hash2(xs)
        known = c * xs + a * c * d + d * e
        # print("is",known)
        # print(min(known))
        # exit(0)
        for i in range(n):
            if abs(known[i]) <= kn_bnd and signals[i] == None:
                signals[i] = wj[i]
                got_signals += 1
    return signals

def collect_signals(n, q, istar, kn_bnd, t):
    global f
    f[0] = 1
    f[istar] = 1

    num_k = q // t

    pool = Pool(num_k)
    signals = pool.starmap(process_k, zip(range(num_k), repeat(n), repeat(kn_bnd), repeat(t)))
    pool.close()
    pool.join()
    # process_k(1, n, kn_bnd, t)
    
    f[istar] = 0
    return signals

def collect_abs(n, q, sigma, istar, kn_bnd, t):
    signals = collect_signals(n, q, istar, kn_bnd, t)
    coeffs_abs = list(range(n))
    for i in range(n):
        f = [signals[k][i] for k in range(q // t)]
        coeffs_abs[i] = helper.likeliest_abs_secret_from_signals_count(f, Ding19.sig, sigma)
        if istar == 0 and coeffs_abs[i] != abs(execution.sj[i]):
            print("error: ", coeffs_abs[i], execution.sj[i], f)
    return coeffs_abs

def attack_step3(n, q, coeffs, MAX_ISTAR):
    sign_comp = [[None for j in range(n)] for istar in range(n)]
    for istar in range(1, MAX_ISTAR):
        for i in range(istar, n):
            if coeffs[0][i] != 0 and coeffs[0][i - istar] != 0:
                if coeffs[istar][i] == coeffs[0][i] + coeffs[0][i - istar]: sign_comp[i][i - istar] = "S"
                else: sign_comp[i][i - istar] = "D"
        for i in range(istar):
            if coeffs[0][i] != 0 and coeffs[0][(i - istar) % n] != 0:
                if coeffs[istar][i] == coeffs[0][i] - coeffs[0][(i - istar) % n]:
                    sign_comp[i][(i - istar) % n] = "S"
                else:
                    sign_comp[i][(i - istar) % n] = "D"
    coeffs_signed = Poly(n, q)
    for i in range(n): coeffs_signed[i] = coeffs[0][i]
    for i in range(0, n):
        if coeffs_signed[i] != 0:
            pos_votes = 0
            neg_votes = 0
            for j in range(0, i):
                if coeffs_signed[j] < 0:
                    if sign_comp[i][j] == "S": neg_votes += 1
                    if sign_comp[i][j] == "D": pos_votes += 1
                elif coeffs_signed[j] > 0:
                    if sign_comp[i][j] == "S": pos_votes += 1
                    if sign_comp[i][j] == "D": neg_votes += 1
            if neg_votes > pos_votes: coeffs_signed[i] *= -1
    return coeffs_signed

def attack(n, q, sigma, a, sj, kn_bnd, kn_bnd2, t, t2):
    coeffs_abs = collect_abs(n, q, sigma, 0, kn_bnd, t)
    print("                           ", strarray(range(n)))
    print("istar = ", 0, "coeffs[istar] = ", strarray(coeffs_abs))
    zero_count = get_zeros(coeffs_abs,n)
    MAX_ISTAR = zero_count + 2
    coeffs = list(range(MAX_ISTAR))
    coeffs[0] = coeffs_abs
    for istar in range(1,MAX_ISTAR):
        coeffs[istar] = collect_abs(n, q, sigma, istar, kn_bnd2, t2)
        print("istar = ", istar, "coeffs[istar] = ", strarray(coeffs[istar]))
    coeffs_signed = attack_step3(n, q, coeffs, MAX_ISTAR)
    print("sB            = ", strarray(sj))
    print("coeffs_signed = ", strarray(coeffs_signed))
    return sj == coeffs_signed or sj == -1 * coeffs_signed

def strarray(a):
    s = "["
    for x in a:
        if isinstance(x, bool): s += " T" if x else " F"
        elif isinstance(x, int): s += "{:2d}".format(x)
        elif isinstance(x, str): s += "{:2s}".format(x)
        elif x is None: s += "  "
        else: assert False, "Invalid type " + x
        s += " "
    s += "]"
    return s

if __name__ == "__main__":
    n = 128
    q = 7580591
    #q = 2255041
    sigma = 3.197 
    kn_bnd = 100000
    kn_bnd2 = 51000
    t1 = 210000
    t2 = 106000
    print("parameters: n = {:d}, q = {:d}, sigma = {:f}".format(n, q, sigma))

    for seed in range(1):
        start = time.time()
        print("======================")
        seed = 1
        print("seed = ", seed)
        #random.seed(seed + time.time())
        random.seed(seed)
        a = Poly.uniform(n, q)
        global f, execution
        f = Poly(n,q)
        execution = Ding19(n, q, sigma)
        sB = execution.sj
        #print("     ", strarray(range(n)))
        print(min(sB),max(sB))
        print("sB = ", strarray(sB))
        #execution.bob()
        print(attack(n,q,sigma,a,sB,kn_bnd,kn_bnd2,t1,t2))
        end = time.time()
        hours, rem = divmod(end-start, 3600)
        minutes, seconds = divmod(rem, 60)
        print("{:0>2}:{:0>2}:{:05.2f}".format(int(hours),int(minutes),seconds))
