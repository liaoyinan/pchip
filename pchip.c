//
// Created by liaoy on 2020/11/10.
//

#include "pchip.h"

#define PCHIP_ABS(a) ((a) > 0 ? (a) : -(a))
static double exteriorSlope(double d1, double d2, double h1, double h2);

inline double exteriorSlope(double d1, double d2, double h1, double h2)
{
    double s;
    double signd1;
    double signs;
    s = ((2.0 * h1 + h2) * d1 - h1 * d2) / (h1 + h2);
    signd1 = d1;
    if (d1 < 0.0)
    {
        signd1 = -1.0;
    }
    else if (d1 > 0.0)
    {
        signd1 = 1.0;
    }
    else
    {
        if (d1 == 0.0)
        {
            signd1 = 0.0;
        }
    }

    signs = s;
    if (s < 0.0)
    {
        signs = -1.0;
    }
    else if (s > 0.0)
    {
        signs = 1.0;
    }
    else
    {
        if (s == 0.0)
        {
            signs = 0.0;
        }
    }

    if (signs != signd1)
    {
        s = 0.0;
    }
    else
    {
        signs = d2;
        if (d2 < 0.0)
        {
            signs = -1.0;
        }
        else if (d2 > 0.0)
        {
            signs = 1.0;
        }
        else
        {
            if (d2 == 0.0)
            {
                signs = 0.0;
            }
        }

        if ((signd1 != signs) && (PCHIP_ABS(s) > PCHIP_ABS(3.0 * d1)))
        {
            s = 3.0 * d1;
        }
    }

    return s;
}

void pchip(const double *x, const double *y, int x_len, const double *new_x, int new_x_len, double *new_y)
{
    int low_ip1;
    double hs;
    double del[x_len - 1];
    double slopes[x_len];
    double h[x_len - 1];
    double hs3;
    double w1;
    int ix;
    double pp_coefs[(x_len - 1) + (3 * (x_len - 1))];
    int low_i;
    int high_i;
    int mid_i;
    for (low_ip1 = 0; low_ip1 < x_len - 1; low_ip1++)
    {
        hs = x[low_ip1 + 1] - x[low_ip1];
        del[low_ip1] = (y[low_ip1 + 1] - y[low_ip1]) / hs;
        h[low_ip1] = hs;
    }

    for (low_ip1 = 0; low_ip1 < x_len - 2; low_ip1++)
    {
        hs = h[low_ip1] + h[low_ip1 + 1];
        hs3 = 3.0 * hs;
        w1 = (h[low_ip1] + hs) / hs3;
        hs = (h[low_ip1 + 1] + hs) / hs3;
        hs3 = 0.0;
        if (del[low_ip1] < 0.0)
        {
            if (del[low_ip1 + 1] <= del[low_ip1])
            {
                hs3 = del[low_ip1] / (w1 * (del[low_ip1] / del[low_ip1 + 1]) + hs);
            }
            else
            {
                if (del[low_ip1 + 1] < 0.0)
                {
                    hs3 = del[low_ip1 + 1] / (w1 + hs * (del[low_ip1 + 1] / del[low_ip1]));
                }
            }
        }
        else
        {
            if (del[low_ip1] > 0.0)
            {
                if (del[low_ip1 + 1] >= del[low_ip1])
                {
                    hs3 = del[low_ip1] / (w1 * (del[low_ip1] / del[low_ip1 + 1]) + hs);
                }
                else
                {
                    if (del[low_ip1 + 1] > 0.0)
                    {
                        hs3 = del[low_ip1 + 1] / (w1 + hs * (del[low_ip1 + 1] / del[low_ip1]));
                    }
                }
            }
        }

        slopes[low_ip1 + 1] = hs3;
    }

    slopes[0] = exteriorSlope(del[0], del[1], h[0], h[1]);
    slopes[x_len - 1] = exteriorSlope(del[x_len - 2], del[x_len - 3], h[x_len - 2], h[x_len - 3]);
    for (low_ip1 = 0; low_ip1 < x_len - 1; low_ip1++)
    {
        hs = (del[low_ip1] - slopes[low_ip1]) / h[low_ip1];
        hs3 = (slopes[low_ip1 + 1] - del[low_ip1]) / h[low_ip1];
        pp_coefs[low_ip1] = (hs3 - hs) / h[low_ip1];
        pp_coefs[low_ip1 + x_len - 1] = 2.0 * hs - hs3;
        pp_coefs[low_ip1 + (2 * (x_len - 1))] = slopes[low_ip1];
        pp_coefs[low_ip1 + (3 * (x_len - 1))] = y[low_ip1];
    }

    for (ix = 0; ix < new_x_len; ix++)
    {
        low_i = 0;
        low_ip1 = 2;
        high_i = x_len;
        while (high_i > low_ip1)
        {
            mid_i = ((low_i + high_i) + 1) >> 1;
            if (new_x[ix] >= x[mid_i - 1])
            {
                low_i = mid_i - 1;
                low_ip1 = mid_i + 1;
            }
            else
            {
                high_i = mid_i;
            }
        }

        hs = new_x[ix] - x[low_i];
        hs3 = pp_coefs[low_i];
        for (low_ip1 = 0; low_ip1 < 3; low_ip1++)
        {
            hs3 = hs * hs3 + pp_coefs[low_i + (low_ip1 + 1) * (x_len - 1)];
        }

        new_y[ix] = hs3;
    }
}
