class Solution(object):
    def findMedianSortedArrays(self, nums1, nums2):
        """
        :type nums1: List[int]
        :type nums2: List[int]
        :rtype: float
        """
        i = 0
        nums1 = nums1 + nums2
        nums1.sort()
        mid = len(nums1)//2
        if(len(nums1)%2):
            return nums1[mid]
        else:
            return (nums1[mid]+nums1[mid-1])/2.0
          
        """
        Runtime of 74ms, beats 88.25% of other submissions
        Memory use of 13.5 MB, beats 94.4% of other submissions
        Compared against 1.7 million other submissions
        """
