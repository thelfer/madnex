# -*- coding: utf-8 -*-

import os
import sys
import unittest
import shutil
import madnex
from madnex.api.api_local import Pivot as Pivot

class TEST_api_local (unittest.TestCase):

    def test_api_macroResuListParam (self):
        """CL_API_000 - On peut lire le nom des macro résultats présents
        """
        d = os.getenv('MADNEX_REFERENCE_FILES_DIRECTORY')
        f = Pivot ('%s/ref_test_api_macroResuWrite_00.edf' %d)
        L_param = f.listMacroResultParam()
        self.assertEqual(L_param, ['P1', 'P2', 'P3'])

    def test_api_macroResuWrite_00 (self):
        """CL_API_002 - On peut écrire des macro-résultats                                     :
        """
        f = Pivot ('test_api_macroResuWrite_00.edf')
        f.writeMacroResult (name = 'P1', libelle = 'param 1', value = '12', unit = '', data_type = 'int32')
        f.writeMacroResult (name = 'P2', libelle = 'param 2', value = '0.75', unit = 'mm', data_type = 'float64')
        f.writeMacroResult (name = 'P3', libelle = 'param 3', value = 'test', unit = '', data_type = 'XXS_string')

    # def test_api_macroResuDelParam_00 (self) :
    #     """CL_API_003 - On peut supprimer un macro-résultat                                    :
    #     """
    #     d = os.getenv('MADNEX_REFERENCE_FILES_DIRECTORY')
    #     shutil.copyfile('%s/ref_test_api_macroResuWrite_00.edf'%d,
    #                     'test_api_macroResuDelParam_00.edf')

    #     f = Pivot ('test_api_macroResuDelParam_00.edf')
        
    #     f.writeMacroResult (name = 'P1', libelle = 'param 1', value = '12', unit = '', data_type = 'int32')
    #     f.writeMacroResult (name = 'P2', libelle = 'param 2', value = '0.75', unit = 'mm', data_type = 'float64')
    #     f.writeMacroResult (name = 'P3', libelle = 'param 3', value = 'test', unit = '', data_type = 'XXS_string')
        
    #     f.deleteMacroResultParam('P2')
    #     L_param = f.listMacroResultParam()
    #     self.assertEqual(L_param, ['P1', 'P2'])

        
if __name__ == '__main__' :
    print('API unit test\n')
    suite = unittest.TestLoader().loadTestsFromTestCase(TEST_api_local)
    r = unittest.TextTestRunner().run(suite)
    sys.exit(not r.wasSuccessful())

